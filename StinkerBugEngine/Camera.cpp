#include "Camera.h"



Camera::Camera(int width, int height, glm::vec3 position) {
	Camera::width = width;
	Camera::height = height;

	transform.position = position;
	transform.rotation = glm::vec3(0.0, 0.0, -1.0);
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
	view = glm::lookAt(transform.position, transform.position + transform.rotation, WorldUp);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	localForward = glm::normalize(transform.rotation);
	right = glm::normalize(glm::cross(localForward, WorldUp));
	localRight = glm::normalize(glm::cross(transform.rotation, localUp));
	localUp = glm::normalize(glm::cross(right, localForward));


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

	vertical = 0.0;
	horizontal = 0.0;
	elevator = 0.0;

	// Horizontal and Lateral Movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { vertical += 1.0; }
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { vertical -= 1.0; }
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { horizontal -= 1.0; }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { horizontal += 1.0; }

	glm::vec3 forward = glm::normalize(glm::vec3(transform.rotation.x, 0.0f, transform.rotation.z));

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { speedMul = 2.0f; }
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { speedMul = 0.5f; }
	else { speedMul = 1.0f; }

	// Up & Down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { elevator += 1; }
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { elevator -= 1; }

	glm::vec3 dir = vertical * forward + horizontal * right + elevator * WorldUp;
	if (glm::length(dir) > 0) { moveDir = glm::normalize(dir); }
	else { moveDir = glm::vec3(0.0); }

	transform.position += moveDir * moveSpeed * speedMul * deltaTime;

	// std::cout << glm::length(moveDir) * speedMul * moveSpeed << "\n";
}

void Camera::Look() {

}