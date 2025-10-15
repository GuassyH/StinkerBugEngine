#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H
#define GLM_ENABLE_EXPERIMENTAL


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Constants.h"

#include "ShadowMapFBO.h"
#include "Component.h"

#include "Texture.h"
#include "Transform.h"


class MeshRenderer;
class Shader;
class Light;
class Scene;

class Camera : public Component {
private:
public:
	GLuint outputFBO = 0;
	GLuint outputRBO = 0;
	bool main = false;

	float FOVdeg = 75.0f;

	float farPlane = 1000.0f;
	float nearPlane = 0.1f;

	int width = 1920;
	int height = 1080;

	bool renderShadows = true;
	bool renderSkybox = true;
	glm::vec4 clearColor = glm::vec4(0.1f, 0.1f, 0.13f, 1.0f);

	glm::mat4 CameraMatrix = glm::mat4(1.0); // M -> *VP*
	glm::vec3 forward = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 localUp = glm::vec3(0.0f, 1.0f, 0.0f);

	Camera() = default;
	Camera(int width, int height);
	void UpdateMatrix();

	void ShadowPass(glm::mat4 light_VP);
	void LightingPass(glm::mat4 light_VP, Light* light);
	void Render(Scene* scene);
	bool CheckOuputFBO(bool forceRewrite);

	ShadowMapFBO m_shadowMapFBO;
	Texture* output_texture = nullptr;
	Texture* old_output_texture = nullptr;

	glm::mat4 getProjectionMat() { return glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane); }
	glm::mat4 getViewMat(){ return transform ? glm::lookAt(transform->position, transform->position + transform->rotation, Constants::Dirs::Up) : glm::mat4(1.0f); }

	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("Camera")) {

			ImGui::Checkbox("Render Shadows", &renderShadows);
			ImGui::Checkbox("Render Skybox", &renderSkybox);
			if (!renderSkybox) {
				ImGui::ColorEdit3("Color", &clearColor.r);
			}
			ImGui::Spacing();	ImGui::Spacing();
			ImGui::DragFloat("FOV", &FOVdeg, 0.1f, 0.0f, 120.0f);
			ImGui::DragFloat("Near Plane", &nearPlane, 0.1f, 0.1f, 2000);
			ImGui::DragFloat("Far Plane", &farPlane, 0.1f, 0.1f, 2000);
		}
	}
};


#endif