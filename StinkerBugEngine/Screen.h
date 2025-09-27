#ifndef SCREEN_NAMESPACE_H
#define SCREEN_NAMESPACE_H

#include <glm/glm.hpp>
#include "ComponentsList.h"
#include "Camera.h"
#include "Scene.h"

#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Physics.h"

namespace Screen
{

	static inline void InitFBO(Camera* camera, unsigned int& fbo, unsigned int& rbo, unsigned int& tex) {
		// Create FBO
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Create Texture and bind
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, camera->width, camera->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

		// Create RBO for depth and stencil we need depth for 3D rendering)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, camera->width, camera->height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "FBO incomplete: " << std::hex << status << std::dec << std::endl;


		// All of the FBO data and such is sent back to the input params
	}

	struct ScreenCastHit {
		bool hit = false;
		Entity entity = 0;
		float distance = 0.0f;
	};



	static inline ScreenCastHit EntityAtMousePos(Camera* camera, Scene& scene, glm::vec2 mousePos, unsigned int& fbo, unsigned int& rbo, unsigned int& tex) {
		ScreenCastHit result;
		result.hit = false;

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glBindTexture(GL_TEXTURE_2D, tex);

		glViewport(0, 0, camera->width, camera->height);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		Shader c_shader("ColToID.vert", "ColToID.frag");
		Material id_material(c_shader, MaterialFlags_Depth);
		glm::vec4 id_color = glm::vec4(0.0f);

		for (auto& [id, components_renderer] : scene.Scene_ECS.WorldRegistry.components[typeid(MeshRenderer)]) {
			MeshRenderer& renderer = *std::static_pointer_cast<MeshRenderer>(components_renderer);
			if (!renderer.model || !renderer.material) { continue; }	// If there isnt a model and material then skip
			Transform& r_transform = scene.Scene_ECS.WorldRegistry.GetComponent<Transform>(id);

			id_color.r = ((id & 0x000000FF) >> 0) / 255.0f;
			id_color.g = ((id & 0x0000FF00) >> 8) / 255.0f;
			id_color.b = ((id & 0x00FF0000) >> 16) / 255.0f;
			id_color.a = 1.0f;

			id_material.Color = id_color;

			renderer.model->render(&id_material, &r_transform, camera->transform, camera, nullptr);
		}

		// Read the pixel at the mouse position
		GLubyte pixel[4];
		glReadPixels(static_cast<GLint>(mousePos.x), static_cast<GLint>(mousePos.y), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
		
		// if pixel is white (255, 255, 255) then its the LAST ID aka no entity (unless you have 16777215 (255^3) entities) 
		if(pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255) {
			result.hit = false;
			result.entity = 0;
		} else {
			unsigned int pickedID = (static_cast<unsigned int>(pixel[0]) << 0) | (static_cast<unsigned int>(pixel[1]) << 8) | (static_cast<unsigned int>(pixel[2]) << 16);
			result.hit = true;
			result.entity = pickedID;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		return result;
	}

	static inline bool IsMouseInRect(glm::vec2 rect_pos, glm::vec2 rect_size) {
		bool result = false;
		double mouseX, mouseY;
		glfwGetCursorPos(Display::getInstance().window, &mouseX, &mouseY);
		if (mouseX >= rect_pos.x && mouseX <= rect_pos.x + rect_size.x &&
			mouseY >= rect_pos.y && mouseY <= rect_pos.y + rect_size.y) {
			result = true;
		}
		return result;
	}

	static inline glm::vec2 GetMousePosInViewport(glm::vec2 rect_pos, glm::vec2 rect_size, glm::vec2 camera_size) {
		double mouseX, mouseY;
		glfwGetCursorPos(Display::getInstance().window, &mouseX, &mouseY);

		// Re origin pos
		mouseX -= rect_pos.x;
		mouseY -= rect_pos.y;

		// Clamp to [0, 1]
		mouseX /= rect_size.x;
		mouseY /= rect_size.y;

		// Rescale to camera size
		mouseX *= camera_size.x;
		mouseY *= camera_size.y;

		// Invert Y axis to match OpenGL coordinates
		mouseY = camera_size.y - mouseY;

		mouseX = glm::clamp(mouseX, 0.0, camera_size.x - 1.0);
		mouseY = glm::clamp(mouseY, 0.0, camera_size.y - 1.0);

		// std::cout << "Mouse Pos in Viewport: " << mouseX << ", " << mouseY << "\n";
		return glm::vec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	}


	static inline Physics::Ray ScreenToWorldRay(glm::vec2 rect_pos, glm::vec2 rect_size, Camera* camera) {
		Physics::Ray ray = Physics::Ray();

		glm::vec2 mouse_pos = GetMousePosInViewport(rect_pos, rect_size, glm::vec2(camera->width, camera->height));

		// Convert to NDC [-1, 1]
		float x = (2.0f * mouse_pos.x) / camera->width - 1.0f;
		float y = 1.0f - (2.0f * mouse_pos.y) / camera->height;

		float fov = glm::radians(camera->FOVdeg);
		float aspect = (float)camera->width / (float)camera->height;
		float scale = tan(fov * 0.5f);

		// Orthonormal basis
		glm::vec3 forward = glm::normalize(camera->forward);
		glm::vec3 right = glm::normalize(camera->right);
		glm::vec3 up = glm::normalize(camera->localUp);

		glm::vec3 rayDir = glm::normalize(forward + x * aspect * scale * right + y * scale * up);


		ray.direction = rayDir;
		ray.position = camera->transform->position;

		Constants::DebugLog::Vec3("Cam Ray Dir", rayDir);
		return ray;
	}
}


#endif