#include "Camera.h"



Camera::Camera(int width, int height, glm::vec3 position) {
	Camera::width = width;
	Camera::height = height;

	transform.position = position;
	transform.rotation = glm::vec3(0.0, 0.0, -1.0);
}


void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane, int windowWidth, int windowHeight) {

	// Initialise the matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// set the width to the windowWidth etc, helps for setting glm::perspective if window size changes
	Camera::width = windowWidth;
	Camera::height = windowHeight;

	// view = lookAt a vec3, projection = perspective I give it
	view = glm::lookAt(transform.position, transform.position + transform.rotation, WorldUp);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	LocalForward = glm::normalize(transform.rotation);
	Right = glm::normalize(glm::cross(LocalForward, WorldUp));
	LocalRight = glm::normalize(glm::cross(transform.rotation, LocalUp));
	LocalUp = glm::normalize(glm::cross(Right, LocalForward));

	Camera::FOVdeg = FOVdeg;
	Camera::farPlane = farPlane;
	Camera::nearPlane = nearPlane;

	camMatrix = projection * view;
}

void Camera::Input() {
	Move();
	Look();
}

void Camera::Move() {
	float deltaTime = DeltaTime::getInstance().get();
	Display& display = Display::getInstance();
	GLFWwindow* window = display.window;

	vertical = glm::vec3(0.0);
	horizontal = glm::vec3(0.0);

	// Horizontal and Lateral Movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		vertical += glm::vec3(glm::normalize(glm::vec4(transform.rotation.x, 0.0f, transform.rotation.z, 0.0f)));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		vertical -= glm::vec3(glm::normalize(glm::vec4(transform.rotation.x, 0.0f, transform.rotation.z, 0.0f)));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		horizontal -= Right;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		horizontal += Right;
	}



	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { speedMul = 2.0f; }
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { speedMul = 0.5f; }
	else { speedMul = 1.0f; }

	// Up & Down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		transform.position += WorldUp * speedMul * moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		transform.position -= WorldUp * speedMul * moveSpeed * deltaTime;
	}


	moveDir = glm::normalize(glm::vec4(horizontal + vertical, 1.0f));
	transform.position += moveDir * moveSpeed * speedMul * deltaTime;

}

void Camera::Look() {

}