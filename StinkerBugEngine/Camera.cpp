#include "Camera.h"

#include "Constants.h"
#include "Display.h"

#include "SceneManager.h"
#include "Scene.h"

#include "ComponentsList.h"
#include "Shader.h"
#include "EntityHelper.h"
#include "FullScreenPass.h"


Shader m_shadowMapShader;

Camera::Camera(int width, int height, Transform& t) {
	Display& display = Display::getInstance();
	GLFWwindow* window = display.window;
	
	Camera::width = width;
	Camera::height = height;

	glfwSetCursorPos(display.window, (width / 2.0f), (height / 2.0f));

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

	/// THIS IS WRONG!!! I forgot that rotation is in degrees not a vector, it works but i should change
	view = glm::lookAt(transform->position, transform->position + transform->rotation, Constants::Dirs::Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	forward = glm::normalize(transform->rotation);
	localUp = glm::normalize(glm::cross(right, forward));
	right = glm::normalize(glm::cross(forward, Constants::Dirs::Up));

	CameraMatrix = projection * view;
}

void Camera::ShadowPass(glm::mat4 light_VP, Light* light) {
	Scene& scene = SceneManager::getInstance().GetActiveScene();

	for (auto& [id, components_renderer] : scene.Scene_ECS.GetComponentMap<MeshRenderer>()) {
		MeshRenderer& renderer = *std::static_pointer_cast<MeshRenderer>(components_renderer);
		if (!renderer.model || !renderer.material) { continue; }	// If there isnt a model and material then skip
		
		Transform& r_transform = scene.Scene_ECS.GetComponent<Transform>(id);
		r_transform.UpdateMatrix();

		m_shadowMapShader.Use();


		// Set the shadow maps light world view proj matrix
		glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader.ID, "light_VP"), 1, GL_FALSE, glm::value_ptr(light_VP));
		glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(r_transform.GetModelMatrix()));

		// Render the scene through the light view
		renderer.model->shadowPass(renderer.material);
	}

}


void Camera::LightingPass(glm::mat4 light_VP, Light* light) {
	Scene& scene = SceneManager::getInstance().GetActiveScene();

	for (auto& [id, components_renderer] : scene.Scene_ECS.GetComponentMap<MeshRenderer>()) {
		MeshRenderer& renderer = *std::static_pointer_cast<MeshRenderer>(components_renderer);
		if (!renderer.model || !renderer.material) { continue; }	// If there isnt a model and material then skip

		Transform& r_transform = scene.Scene_ECS.GetComponent<Transform>(id);

		renderer.model->render(renderer.material, &r_transform, transform, this, light);
	}
}


void Camera::Render(Scene* scene) {

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glm::mat4 light_VP = glm::mat4(1.0f);
	Transform* l_transform = nullptr;

	if (scene->HasMainLight()) { 
		l_transform = &scene->main_light->GetComponent<Transform>();


		glm::vec3 direction = l_transform->DegToRad();

		scene->main_light->GetComponent<Light>().vec_direction = direction;

		glm::mat4 lightProj = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 200.0f);
		glm::mat4 lightView = glm::lookAt(transform->position - (direction * glm::vec3(100)), transform->position - (direction * glm::vec3(100)) + direction, Constants::Dirs::Up);
		glm::mat4 light_VP = lightProj * lightView;
		
		scene->main_light->GetComponent<Light>().light_VP = light_VP;
		
		// Bind shadow map and draw scene to it
		m_shadowMapFBO.BindForWriting();
		glClear(GL_DEPTH_BUFFER_BIT);  // clear depth before shadow pass
		ShadowPass(light_VP, &scene->main_light->GetComponent<Light>());
	}


	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	if (!output_texture) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, width, height);
	}
	else {
		if (!CheckOuputFBO(false)) { std::cout << "The output_texture is null or FBO incomplete" << std::endl; return; }
		glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, output_texture->imgWidth, output_texture->imgHeight);
	}



	for (FullScreenPass pass : scene->passes) {
		if (scene->HasMainLight()) {
			pass.Draw(*this, &scene->main_light->GetComponent<Light>(), &scene->main_light->GetComponent<Transform>());
		}
		else {
			pass.Draw(*this, nullptr, nullptr);
		}
	}

	if (scene->HasMainLight()) {
		// Do lighting pass and bind to base FBO (0) Technically dont bind shadowFBO now but its wtv
		m_shadowMapFBO.BindForReading(GL_TEXTURE0);
		LightingPass(light_VP, &scene->main_light->GetComponent<Light>());
	}
	else {
		LightingPass(light_VP, nullptr);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


bool Camera::CheckOuputFBO(bool forceRewrite) {
	if (!output_texture) return false;

	// If rewrite isnt forced then check first otherwise force a reqrite
	if (!forceRewrite) {
		if (old_output_texture == output_texture &&	// the texture hasnt changed
			output_texture->imgWidth == width &&	// the width is the same as the camera's
			output_texture->imgHeight == height)	// the height is the same as the camera's
		{ return true; }
	}

	// Delete old buffers & textures if needed
	if (outputFBO) { glDeleteFramebuffers(1, &outputFBO); outputFBO = 0; }
	if (outputRBO) { glDeleteRenderbuffers(1, &outputRBO); outputRBO = 0; }
	if (output_texture && output_texture->ID && output_texture->ID != m_shadowMapFBO.m_shadowMap) 
	{ GLuint id = output_texture->ID; glDeleteTextures(1, &id); output_texture->ID = 0; }

	old_output_texture = output_texture;

	// store sizes so texture struct is honest
	output_texture->imgWidth = width;
	output_texture->imgHeight = height;

	// generate & bind FBO (separate GLuint)
	glGenFramebuffers(1, &outputFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);

	// generate texture for color attachment
	glGenTextures(1, &output_texture->ID);
	glBindTexture(GL_TEXTURE_2D, output_texture->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output_texture->ID, 0);

	// renderbuffer for depth+stencil
	glGenRenderbuffers(1, &outputRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, outputRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, outputRBO);

	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "output_texture FBO not complete! : " << std::hex << fboStatus << std::dec << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}

	std::cout << "shadow fbo tex: " << m_shadowMapFBO.m_shadowMap << " - output_tex: " << output_texture->ID << "\n";

	// unbind to be safe
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

