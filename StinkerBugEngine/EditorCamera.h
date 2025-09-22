#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H


#include "Transform.h"
#include "Camera.h"


class EditorCamera {
public:
	Transform* transform = nullptr;
	Camera* camera = nullptr;
	
	EditorCamera() = default;

	float moveSpeed = 2.0f;
	float speedMul = 1.0f;

	float sensitivity = 2;

	float horizontal, vertical, elevator = 0;
	glm::vec3 moveDir = glm::vec3(0.0f);

	void Move() {
		float deltaTime = DeltaTime::getInstance().get();
		Display& display = Display::getInstance();
		GLFWwindow* window = display.window;

		vertical = 0.0;
		horizontal = 0.0;
		elevator = 0.0;

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

		glm::vec3 dir = vertical * proj_forward + horizontal * camera->right + elevator * Constants::Dirs::Up;
		if (glm::length(dir) > 0) { moveDir = glm::normalize(dir); }
		else { moveDir = glm::vec3(0.0); }

		this->transform->position += moveDir * moveSpeed * speedMul * deltaTime;

	}

	bool firstClick = false;
	void Look() {
		float deltaTime = DeltaTime::getInstance().get();
		Display& display = Display::getInstance();
		GLFWwindow* window = display.window;

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * ((float)(mouseY - (camera->height / 2)) / (float)camera->height) * 100;
		float rotY = sensitivity * ((float)(mouseX - (camera->width / 2)) / (float)camera->width) * 100;


		glm::vec3 newOrientation = glm::rotate(this->transform->rotation, glm::radians(-rotX), glm::normalize(glm::cross(this->transform->rotation, Constants::Dirs::Up)));
		if (!((glm::angle(newOrientation, Constants::Dirs::Up) <= glm::radians(5.0f)) || (glm::angle(newOrientation, -Constants::Dirs::Up) <= glm::radians(5.0f)))) {
			this->transform->rotation = newOrientation;
		}
		this->transform->rotation = glm::rotate(this->transform->rotation, glm::radians(-rotY), Constants::Dirs::Up);

		glfwSetCursorPos(window, (camera->width / 2.0f), (camera->height / 2.0f));
		
	}
};



#endif