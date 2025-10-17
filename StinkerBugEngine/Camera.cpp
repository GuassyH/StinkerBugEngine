#include "Camera.h"

#include "Display.h"

#include "SceneManager.h"
#include "Scene.h"

#include "ComponentsList.h"
#include "Shader.h"
#include "EntityObject.h"
#include "FullScreenPass.h"
#include "Screen.h"

#include "Renderer.h"

Shader m_shadowMapShader;

Camera::Camera(int width, int height) {
	Display& display = Display::getInstance();
	GLFWwindow* window = display.window;
	
	Camera::width = width;
	Camera::height = height;

	glfwSetCursorPos(display.window, (width / 2.0f), (height / 2.0f));
	
	// Should maybe be case by case
	m_shadowMapFBO.Init(8192, 8192);
	m_shadowMapShader = Shader("ShadowMapFBO.vert", "ShadowMapFBO.frag");
}


void Camera::UpdateMatrix() {
	if (!transform) { return; }
	
	// Initialise the matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	/// THIS IS WRONG!!! I forgot that rotation is in degrees not a vector, it works but i should change
	view = glm::lookAt(transform->position, transform->position + transform->DegToVec(), Constants::Dirs::Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	forward = glm::normalize(transform->DegToVec());
	localUp = glm::normalize(glm::cross(right, forward));
	right = glm::normalize(glm::cross(forward, Constants::Dirs::Up));

	CameraMatrix = projection * view;
}

void Camera::ShadowPass(glm::mat4 light_VP) {
	if (!renderShadows) { return; }
	for (auto& call : Renderer::getInstance().opaque_meshes) {
		if (!call.renderer->shadowCaster || !call.renderer->material->HasFlag(MaterialFlags_Shadow) || !call.transform || !call.renderer) { continue; }

		// SHOULD BE DONE ONLY ONCE A FRAME? NOT PER CAMERA
		call.transform->UpdateMatrix();

		m_shadowMapShader.Use();

		// Set the shadow maps light world view proj matrix
		glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader.ID, "light_VP"), 1, GL_FALSE, glm::value_ptr(light_VP));
		glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(call.transform->GetModelMatrix()));

		// Render the scene through the light view
		call.renderer->model->shadowPass();
	}
	for (auto& call : Renderer::getInstance().transparent_meshes) {
		if (!call.renderer->shadowCaster || !call.renderer->material->HasFlag(MaterialFlags_Shadow) || !call.transform || !call.renderer) { continue; }

		// SHOULD BE DONE ONLY ONCE A FRAME? NOT PER CAMERA
		call.transform->UpdateMatrix();

		m_shadowMapShader.Use();

		// Set the shadow maps light world view proj matrix
		glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader.ID, "light_VP"), 1, GL_FALSE, glm::value_ptr(light_VP));
		glUniformMatrix4fv(glGetUniformLocation(m_shadowMapShader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(call.transform->GetModelMatrix()));

		// Render the scene through the light view
		call.renderer->model->shadowPass();
	}
}


void Camera::LightingPass(glm::mat4 light_VP, Light* light) {
	// Opaque
	for (auto& call : Renderer::getInstance().opaque_meshes) {
		if (!call.transform || !call.renderer) { continue; }
		call.renderer->model->render(call.renderer->material, call.transform.lock(), this, light);
	}

	Renderer::getInstance().sortTransparentMeshes(this->transform->position);

	// Transparent Not fully working
	for (auto& call : Renderer::getInstance().transparent_meshes) {
		if (!call.transform || !call.renderer) { continue; }
		call.renderer->model->render(call.renderer->material, call.transform.lock(), this, light);
	}
}


void Camera::Render(Scene* scene) {
	if (!this->transform) { std::cout << "No Transform" << std::endl; return; }

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	UpdateMatrix();

	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glm::mat4 light_VP = glm::mat4(1.0f);

	if (scene->HasMainLight()) {
		std::weak_ptr<Transform> weak_l_transform = scene->main_light->transform->GetComponentPtr<Transform>();
		if (auto l_transform = weak_l_transform.lock()) {
			glm::vec3 direction;
			direction = l_transform->DegToVec();

			float light_map_size = 50.0f;
			glm::mat4 lightProj = glm::ortho(-light_map_size, light_map_size, -light_map_size, light_map_size, 0.1f, 200.0f);
			glm::mat4 lightView = glm::lookAt(this->transform->position - (direction * glm::vec3(100)), this->transform->position - (direction * glm::vec3(100)) + direction, Constants::Dirs::Up);
			glm::mat4 light_VP = lightProj * lightView;
		
			l_transform->GetComponent<Light>().light_VP = light_VP;
		
			// Create a buffer of all lights and send to frag shader

			// Bind shadow map and draw scene to it
			m_shadowMapFBO.BindForWriting();
			glClear(GL_DEPTH_BUFFER_BIT);  // clear depth before shadow pass
			ShadowPass(light_VP);
		}
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


	// should do opaque then transparent stuff
	if (scene->HasMainLight()) {
		if (renderSkybox) { scene->skybox_pass.Draw(*this, scene->main_light->transform->GetComponentPtr<Light>(), scene->main_light->transform.lock()); }
		for (FullScreenPass pass : scene->passes) {
			pass.Draw(*this, scene->main_light->transform->GetComponentPtr<Light>(), scene->main_light->transform.lock());
		}

		m_shadowMapFBO.BindForReading(GL_TEXTURE0);
		LightingPass(light_VP, &scene->main_light->transform->GetComponent<Light>());
	}
	else {
		if (renderSkybox) { scene->skybox_pass.Draw(*this, nullptr, nullptr); }
		for (FullScreenPass pass : scene->passes) {
			pass.Draw(*this, scene->main_light->transform->GetComponentPtr<Light>(), scene->main_light->transform.lock());
		}
		
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

	output_texture->imgWidth = width;
	output_texture->imgHeight = height;

	Screen::InitFBO(this, outputFBO, outputRBO, output_texture->ID);

	return true;
}

