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

#include "Transform.h"
#include "ShadowMapFBO.h"

class MeshRenderer;
class Shader;
class Light;

class Camera {
public:
	Transform* transform = nullptr;
	bool main = false;

	float FOVdeg = 75.0f;

	float farPlane = 1000.0f;
	float nearPlane = 0.1f;

	int width = 1920;
	int height = 1080;

	glm::mat4 CameraMatrix = glm::mat4(1.0); // M -> *VP*

	glm::vec3 forward = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 localUp = glm::vec3(0.0f, 1.0f, 0.0f);

	Camera() = default;
	Camera(int width, int height, Transform& t);
	void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane, int windowWidth, int windowHeight);


	void ShadowPass(glm::mat4 light_MVP);
	void LightingPass(glm::mat4 light_MVP);
	void Render(Light& light, Transform& l_transform);
	

	ShadowMapFBO m_shadowMapFBO;
};


#endif