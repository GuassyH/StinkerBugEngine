#include "Camera.h"
#include "Constants.h"

#include "Display.h"

inline glm::vec3 WorldUp = glm::vec3(0.0, 1.0, 0.0);

Camera::Camera(int width, int height, Transform& t) {
	Display& display = Display::getInstance();
	GLFWwindow* window = display.window;
	
	Camera::width = width;
	Camera::height = height;

	glfwSetCursorPos(display.window, (width / 2), (height / 2));

	transform = &t;
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
	view = glm::lookAt(transform->position, transform->position + transform->rotation, WorldUp);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	forward = glm::normalize(transform->rotation);
	localUp = glm::normalize(glm::cross(right, forward));
	right = glm::normalize(glm::cross(forward, Constants::Dirs::Up));

	// std::cout << t.rotation.x << "x " << t.rotation.y << "y " << t.rotation.z << "z\n";
	CameraMatrix = projection * view;

}
