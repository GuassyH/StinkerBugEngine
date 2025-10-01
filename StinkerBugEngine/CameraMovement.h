#pragma once

#include "EntityBehaviour.h"
#include "Entity.h"
#include "Display.h"
#include "Camera.h"
#include "EntityHelper.h"

class CameraMovement : public EntityBehaviour {
public:
	float FOVdeg = 60.0f;
	float farPlane = 1000.0f;
	float nearPlane = 0.1f;

	float moveSpeed = 2.0f;
	float speedMul = 1.0f;

	float sensitivity = 2;

	float horizontal, vertical, elevator = 0;
	glm::vec3 moveDir = glm::vec3(0.0f);

	bool focusMouse = true;
	Camera* camera = nullptr;

	glm::vec2 w_size;
	glm::vec2 w_pos;

	void Move() {
		float deltaTime = DeltaTime::getInstance().get();
		Display& display = Display::getInstance();
		GLFWwindow* window = display.window;



		vertical = 0.0;
		horizontal = 0.0;
		elevator = 0.0;

		if (focusMouse) {
			// Horizontal and Lateral Movement
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { vertical += 1.0; }
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { vertical -= 1.0; }
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { horizontal -= 1.0; }
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { horizontal += 1.0; }

			glm::vec3 proj_forward = glm::normalize(camera->forward * glm::vec3(1, 0, 1));

			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { speedMul = 2.0f; }
			else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { speedMul = 0.5f; }
			else { speedMul = 1.0f; }

			// Up & Down
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { elevator += 1; }
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { elevator -= 1; }

			// Use cam forward for freecam
			glm::vec3 dir = vertical * proj_forward + horizontal * camera->right + elevator * Constants::Dirs::Up;
			if (glm::length(dir) > 0) { moveDir = glm::normalize(dir); }
			else { moveDir = glm::vec3(0.0); }

			this->transform->position += moveDir * moveSpeed * speedMul * deltaTime;

		}
	}

	bool firstClick = false;
	void Look() {
		float deltaTime = DeltaTime::getInstance().get();
		Display& display = Display::getInstance();
		GLFWwindow* window = display.window;

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		double centerX = glm::roundEven(w_pos.x + (w_size.x / 2.0f));
		double centerY = glm::roundEven(w_pos.y + (w_size.y / 2.0f));

		float deltaX = (float)(mouseX - centerX) / w_size.x;
		float deltaY = (float)(mouseY - centerY) / w_size.y;

		float rotX = -deltaY * sensitivity * 100.0f;
		float rotY = -deltaX * sensitivity * 100.0f;

		// Apply rotations to Euler angles
		transform->rotation.x -= rotX; // pitch
		transform->rotation.y += rotY; // yaw

		// Clamp pitch to avoid flipping
		if (transform->rotation.x > 89.0f) transform->rotation.x = 89.0f;
		if (transform->rotation.x < -89.0f) transform->rotation.x = -89.0f;

		// Scroll wheel speed adjust
		if (display.scroll != 0) {
			moveSpeed = glm::max(0.5f, moveSpeed + (display.scroll > 0 ? 0.5f : -0.5f));
		}


		glfwSetCursorPos(window, centerX, centerY);
	}

	void Start() override {
		// camera = &entityHelper->GetComponent<Camera>();
		camera = &entityHelper->GetComponent<Camera>();
		
		Display& display = Display::getInstance();
		glfwSetCursorPos(display.window, (camera->width / 2.0f), (camera->height / 2.0f));
		this->transform->rotation = glm::vec3(0.0, 0.0, -1.0f);
	}

	void Update() override {
		Move();
		Look();
	}

};