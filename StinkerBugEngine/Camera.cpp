#include "Camera.h"
#include "Constants.h"


glm::vec3& WorldUp = Constants::Dirs::Up;

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

	forward = glm::normalize(transform.rotation);
	localUp = glm::normalize(glm::cross(right, forward));	
	right = glm::normalize(glm::cross(forward, WorldUp));


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

	glm::vec3 proj_forward = glm::normalize(transform.rotation * glm::vec3(1, 0, 1));

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { speedMul = 2.0f; }
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { speedMul = 0.5f; }
	else { speedMul = 1.0f; }

	// Up & Down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { elevator += 1; }
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { elevator -= 1; }

	glm::vec3 dir = vertical * proj_forward + horizontal * right + elevator * WorldUp;
	if (glm::length(dir) > 0) { moveDir = glm::normalize(dir); }
	else { moveDir = glm::vec3(0.0); }

	transform.position += moveDir * moveSpeed * speedMul * deltaTime;

}

bool firstClick = false;
void Camera::Look() {
	float deltaTime = DeltaTime::getInstance().get();
	Display& display = Display::getInstance();
	GLFWwindow* window = display.window;


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (firstClick) {
			glfwSetCursorPos(window, (width / 2), (height / 2));
			focusMouse = !focusMouse;
			firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) { firstClick = true; }

	if (focusMouse) {

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * ((float)(mouseY - (height / 2)) / (float)height) * 100;
		float rotY = sensitivity * ((float)(mouseX - (width / 2)) / (float)width) * 100;


		glm::vec3 newOrientation = glm::rotate(transform.rotation, glm::radians(-rotX), glm::normalize(glm::cross(transform.rotation, WorldUp)));
		if (!((glm::angle(newOrientation, WorldUp) <= glm::radians(5.0f)) || (glm::angle(newOrientation, -WorldUp) <= glm::radians(5.0f)))) {
			transform.rotation = newOrientation;
		}
		transform.rotation = glm::rotate(transform.rotation, glm::radians(-rotY), WorldUp);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
}