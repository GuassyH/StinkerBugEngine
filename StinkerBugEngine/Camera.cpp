#include "Camera.h"

#include "Constants.h"
#include "Display.h"

#include "SceneManager.h"
#include "Scene.h"

#include "MeshRenderer.h"
#include "Mesh.h"
#include "Light.h"
#include "Shader.h"

inline glm::vec3 WorldUp = glm::vec3(0.0, 1.0, 0.0);

Shader m_shadowMapShader;


Camera::Camera(int width, int height, Transform& t) {
	Display& display = Display::getInstance();
	GLFWwindow* window = display.window;
	
	Camera::width = width;
	Camera::height = height;

	glfwSetCursorPos(display.window, (width / 2), (height / 2));

	// m_shadowMapFBO.Init(1024, 1024);
	m_shadowMapShader = Shader("ShadowMapFBO.vert", "ShadowMapFBO.frag");
	transform = &t;
}


void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane, int windowWidth, int windowHeight) {
	// set the width to the windowWidth etc, helps for setting glm::perspective if window size changes
	Camera::width = windowWidth;
	Camera::height = windowHeight;

	Camera::FOVdeg = FOVdeg;
	Camera::farPlane = farPlane;
	Camera::nearPlane = nearPlane;

	// Initialise the matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// view = lookAt a vec3, projection = perspective I give it
	view = glm::lookAt(transform->position, transform->position + transform->rotation, WorldUp);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	forward = glm::normalize(transform->rotation);
	localUp = glm::normalize(glm::cross(right, forward));
	right = glm::normalize(glm::cross(forward, Constants::Dirs::Up));

	// std::cout << t.rotation.x << "x " << t.rotation.y << "y " << t.rotation.z << "z\n";
	CameraMatrix = projection * view;

}


void Camera::ShadowPass(MeshRenderer& renderer, Transform& r_transform, Light& light, Transform& l_transform) {
	// std::cout << "Shadow Pass\n";
	Mesh& r_mesh = *renderer.mesh;
	r_mesh.UpdateMatrices(r_transform);

	// Bind the shadow map FrameBuffer for writing
	m_shadowMapFBO.BindForWriting();

	// Clear the depth buffer
	// glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);


	m_shadowMapShader.Use();

	// Calculate the lights MVP
	glm::mat4 lightProj = glm::perspective(glm::radians(40.0f), 1.0f, 0.1f, 100.0f);
	glm::mat4 lightView = glm::lookAt(l_transform.position, l_transform.position + l_transform.rotation, WorldUp);
	lightView = glm::translate(lightView, l_transform.position);
	glm::mat4 light_MVP = lightProj * lightView * r_mesh.modelMatrix;

	// Set the shadow maps light world view proj matrix
	glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader.ID, "light_WVP"), 1, GL_FALSE, glm::value_ptr(light_MVP));

	// Render the scene through the light view
	renderer.mesh->VAO1.Bind();
	glDrawElements(GL_TRIANGLES, renderer.mesh->indices.size(), GL_UNSIGNED_INT, 0);

}

void Camera::LightingPass(MeshRenderer& renderer, Transform& r_transform) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, width, height);

	Shader& r_shader = renderer.material->shader;
	Mesh& r_mesh = *renderer.mesh;
	r_mesh.UpdateMatrices(r_transform);
	r_shader.Use();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	// m_shadowMapFBO.BindForReading(GL_TEXTURE0);


	glUniformMatrix4fv(glGetUniformLocation(r_shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(r_mesh.modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(r_shader.ID, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(r_mesh.rotationMatrix));
	glUniformMatrix4fv(glGetUniformLocation(r_shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(CameraMatrix));
	glUniform3f(glGetUniformLocation(r_shader.ID, "camPos"), transform->position.x, transform->position.y, transform->position.z);
	glUniform4f(glGetUniformLocation(r_shader.ID, "color"), renderer.material->Color.r, renderer.material->Color.g, renderer.material->Color.b, renderer.material->Color.a);

	if (renderer.material->Lit && SceneManager::getInstance().GetActiveScene()) {
		glm::vec3 l_dir = SceneManager::getInstance().GetActiveScene()->light_direction;
		glm::vec3 l_col = SceneManager::getInstance().GetActiveScene()->light_color;
		glUniform3f(glGetUniformLocation(r_shader.ID, "lightDir"), l_dir.x, l_dir.y, l_dir.z);
		glUniform4f(glGetUniformLocation(r_shader.ID, "lightColor"), l_col.r, l_col.g, l_col.b, 1.0f);
		glUniform1f(glGetUniformLocation(r_shader.ID, "ambient"), SceneManager::getInstance().GetActiveScene()->ambient);
	}

	renderer.mesh->VAO1.Bind();
	glDrawElements(GL_TRIANGLES, renderer.mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Camera::Render(Light& light, Transform& l_transform) {
	Scene* scene = SceneManager::getInstance().GetActiveScene();

	for (auto& [id, renderer] : scene->Scene_ECS.mesh_renderers) {
		auto it = scene->Scene_ECS.transforms.find(id);
		Transform& t = it->second;

		// ShadowPass(renderer, t, light, l_transform);
		LightingPass(renderer, t);
	}
}