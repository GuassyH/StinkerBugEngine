#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Transform.h"
#include "DeltaTime.h"
#include "Display.h"

class Camera {
public:
	Transform transform;

	glm::mat4 view = glm::mat4(1.0);
	glm::mat4 proj = glm::mat4(1.0);
	glm::mat4 camMatrix = glm::mat4(1.0);

	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 LocalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 LocalForward = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 LocalRight = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float FOVdeg = 60.0f;

	float farPlane = 1000.0f;
	float nearPlane = 0.1f;


	Camera(int width, int height, glm::vec3 position);
	void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane, int windowWidth, int windowHeight);
	void Input();
	void Move();
	void Look();

	int width, height;
	glm::vec3 horizontal, vertical;
	glm::vec3 moveDir;

	float moveSpeed = 1;
	float speedMul = 1;
};


#endif