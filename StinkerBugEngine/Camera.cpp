#include "Camera.h"

#include "Constants.h"
#include "Display.h"

#include "SceneManager.h"
#include "Scene.h"

#include "ComponentsList.h"
#include "Shader.h"

inline glm::vec3 WorldUp = glm::vec3(0.0, 1.0, 0.0);

Shader m_shadowMapShader;


Camera::Camera(int width, int height, Transform& t) {
	Display& display = Display::getInstance();
	GLFWwindow* window = display.window;
	
	Camera::width = width;
	Camera::height = height;

	glfwSetCursorPos(display.window, (width / 2), (height / 2));

	m_shadowMapFBO.Init(8192, 8192);
	m_shadowMapShader = Shader("ShadowMapFBO.vert", "ShadowMapFBO.frag");
	transform = &t;
}


void Camera::UpdateMatrix(int windowWidth, int windowHeight) {
	// set the width to the windowWidth etc, helps for setting glm::perspective if window size changes
	Camera::width = windowWidth;
	Camera::height = windowHeight;

	Camera::FOVdeg = FOVdeg;
	Camera::farPlane = farPlane;
	Camera::nearPlane = nearPlane;

	// Initialise the matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// THIS IS WRONG!!! I forgot that rotation is in degrees not a vector, it works but i should change
	view = glm::lookAt(transform->position, transform->position + transform->rotation, WorldUp);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	forward = glm::normalize(transform->rotation);
	localUp = glm::normalize(glm::cross(right, forward));
	right = glm::normalize(glm::cross(forward, Constants::Dirs::Up));

	// std::cout << t.rotation.x << "x " << t.rotation.y << "y " << t.rotation.z << "z\n";
	CameraMatrix = projection * view;
}

void Camera::ShadowPass(glm::mat4 light_MVP) {
	Scene& scene = SceneManager::getInstance().GetActiveScene();

	for (auto& [id, renderer] : scene.Scene_ECS.mesh_renderers) {
		auto it = scene.Scene_ECS.transforms.find(id);
		Transform& r_transform = it->second;
		// std::cout << "Shadow Pass\n";
		Mesh& r_mesh = *renderer.mesh;
		r_transform.UpdateMatrix();

		m_shadowMapShader.Use();


		// Set the shadow maps light world view proj matrix
		glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader.ID, "light_WVP"), 1, GL_FALSE, glm::value_ptr(light_MVP));
		glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(r_transform.GetModelMatrix()));

		// Render the scene through the light view
		renderer.mesh->VAO1.Bind();
		glDrawElements(GL_TRIANGLES, renderer.mesh->indices.size(), GL_UNSIGNED_INT, 0);
	}

}


void Camera::LightingPass(glm::mat4 light_MVP) {
	Scene& scene = SceneManager::getInstance().GetActiveScene();
	
	for (auto& [id, renderer] : scene.Scene_ECS.mesh_renderers) {
		auto it = scene.Scene_ECS.transforms.find(id);
		Transform& r_transform = it->second;


		Shader& r_shader = renderer.material->shader;
		Mesh& r_mesh = *renderer.mesh;
		r_transform.UpdateMatrix();
		r_shader.Use();

		glUniform1i(glGetUniformLocation(r_shader.ID, "ShadowMap"), 0);
		glUniformMatrix4fv(glGetUniformLocation(r_shader.ID, "light_WVP"), 1, GL_FALSE, glm::value_ptr(light_MVP));
		glUniformMatrix4fv(glGetUniformLocation(r_shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(r_transform.GetModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(r_shader.ID, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(r_transform.GetRotationMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(r_shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(CameraMatrix));
		glUniform3f(glGetUniformLocation(r_shader.ID, "camPos"), transform->position.x, transform->position.y, transform->position.z);
		glUniform4f(glGetUniformLocation(r_shader.ID, "color"), renderer.material->Color.r, renderer.material->Color.g, renderer.material->Color.b, renderer.material->Color.a);


		if (renderer.material->Lit) {
			glm::vec3 l_dir = SceneManager::getInstance().GetActiveScene().light_direction;
			glm::vec3 l_col = SceneManager::getInstance().GetActiveScene().light_color;
			glUniform3f(glGetUniformLocation(r_shader.ID, "lightDir"), l_dir.x, l_dir.y, l_dir.z);
			glUniform4f(glGetUniformLocation(r_shader.ID, "lightColor"), l_col.r, l_col.g, l_col.b, 1.0f);
			glUniform1f(glGetUniformLocation(r_shader.ID, "ambient"), SceneManager::getInstance().GetActiveScene().ambient);
		}

		renderer.mesh->VAO1.Bind();
		glDrawElements(GL_TRIANGLES, renderer.mesh->indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void Camera::Render(Light& light, Transform& l_transform) {
	float pitch = glm::radians(l_transform.rotation.x);
	float yaw = glm::radians(l_transform.rotation.y);

	glm::vec3 direction;
	direction.x = cos(pitch) * cos(yaw);
	direction.y = sin(pitch);
	direction.z = cos(pitch) * sin(yaw);
	glm::vec3 lightViewDir = l_transform.rotation;

	glm::mat4 lightProj = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 200.0f);
	glm::mat4 lightView = glm::lookAt(transform->position + l_transform.position, transform->position + l_transform.position + lightViewDir, WorldUp);
	glm::mat4 light_MVP = lightProj * lightView;

	// std::cout << "Light rot : " << ": " << lightViewDir.x << "x " << lightViewDir.y << "y " << lightViewDir.z << "z\n";
	
	m_shadowMapFBO.BindForWriting();    
	ShadowPass(light_MVP);              
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	glViewport(0, 0, width, height);         
	m_shadowMapFBO.BindForReading(GL_TEXTURE0); 

	LightingPass(light_MVP);                 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);   

}