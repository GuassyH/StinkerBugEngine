#include "EditorCamera.h"
#include "Scene.h"
#include "Screen.h"
#include "Physics.h"

void EditorCamera::Init() {
	transform = new Transform();
	transform->rotation = glm::vec3(0, 0, -1);
	camera = new Camera(1920, 1080, *transform);
	camera->FOVdeg = 90.0f;
	camera->farPlane = 1000.0f;
	camera->nearPlane = 0.1f;
	camera->CheckOuputFBO(true);

	Screen::InitFBO(camera, select_fbo, select_rbo, select_tex);
}

void EditorCamera::AddGizmoEntities(Scene& scene) {
	Gizmos::Gizmo transform_gizmo = Gizmos::TransformHandle(scene.Scene_ECS);
	Gizmos::Gizmo scale_gizmo = Gizmos::ScaleHandle(scene.Scene_ECS);
	Gizmos::Gizmo rotate_gizmo = Gizmos::RotateHandle(scene.Scene_ECS);

	gizmos.push_back(transform_gizmo);
	gizmos.push_back(scale_gizmo);
	gizmos.push_back(rotate_gizmo);
}

void EditorCamera::DrawGizmos(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Rebind the framebuffer to the editor camera's FBO
	glBindFramebuffer(GL_FRAMEBUFFER, camera->outputFBO);

	selected_entity_helper.registry = &scene.Scene_ECS.WorldRegistry;
	selected_entity_helper.id = selected_entity;

	// If there isnt an entity selected then skip drawing gizmos, reset framebuffer 
	if (!is_entity_selected || scene.Scene_ECS.WorldRegistry.entities.find(selected_entity) == scene.Scene_ECS.WorldRegistry.entities.end()) { glDisable(GL_BLEND); glBindFramebuffer(GL_FRAMEBUFFER, 0); return; }

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	if (glfwGetMouseButton(Display::getInstance().window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
		if (glfwGetKey(Display::getInstance().window, GLFW_KEY_W) == GLFW_PRESS) { selected_gizmo = 0; }
		if (glfwGetKey(Display::getInstance().window, GLFW_KEY_S) == GLFW_PRESS) { selected_gizmo = 1; }
		if (glfwGetKey(Display::getInstance().window, GLFW_KEY_R) == GLFW_PRESS) { selected_gizmo = 2; }
	}
	gizmos[selected_gizmo].Draw(camera, scene, transform, selected_entity_helper);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_BLEND);
}

void EditorCamera::SelectObject(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {
	


	// If you left click
	if (glfwGetMouseButton(Display::getInstance().window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && glfwGetMouseButton(Display::getInstance().window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) { 
		if (firstClick) {
			firstClick = false;
			if (Screen::IsMouseInRect(w_pos, w_size)) {
				// Cast a ray from the mouse position
				Screen::ScreenCastHit scHit = Screen::EntityAtMousePos
				(camera, scene, Screen::GetMousePosInViewport(w_pos, w_size, glm::vec2(camera->width, camera->height)), select_fbo, select_rbo, select_tex);
				
				Physics::RaycastHit rayHit = Physics::Raycast(Screen::ScreenToWorldRay(w_pos, w_size, camera), camera->farPlane);

				if (!rayHit.isGizmo) {
					if (scHit.hit) {
						selected_entity = scHit.entity;
						is_entity_selected = true;
					}
					else {
						is_entity_selected = false;
					}
				}
				else {
					// Do Gizmo stuff
					EntityHelper gizmo(rayHit.entity, &scene.Scene_ECS.EditorRegistry);
					gizmo.GetComponent<GizmoComponent>().isHovered = true;
				}
			}
		}
	}
	else {
		firstClick = true;
	}
}



void EditorCamera::Move() {
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

	glm::vec3 dir = vertical * camera->forward + horizontal * camera->right + elevator * Constants::Dirs::Up;
	if (glm::length(dir) > 0) { moveDir = glm::normalize(dir); }
	else { moveDir = glm::vec3(0.0); }

	this->transform->position += moveDir * moveSpeed * speedMul * deltaTime;

}


void EditorCamera::Look() {
	float deltaTime = DeltaTime::getInstance().get();
	Display& display = Display::getInstance();
	GLFWwindow* window = display.window;

	// Get mouse position in screen coords
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// Compute mouse delta relative to image center
	double centerX = glm::roundEven(w_pos.x + (w_size.x / 2.0f));
	double centerY = glm::roundEven(w_pos.y + (w_size.y / 2.0f));

	float deltaX = (float)(mouseX - centerX);
	float deltaY = (float)(mouseY - centerY);

	// Convert to [-1,1] range based on image size (optional)
	deltaX /= w_size.x;
	deltaY /= w_size.y;

	// Apply sensitivity 
	float rotX = -deltaY * sensitivity * 100.0f;
	float rotY = -deltaX * sensitivity * 100.0f;

	glm::vec3 newOrientation = glm::rotate(this->transform->rotation, glm::radians(rotX), glm::normalize(glm::cross(this->transform->rotation, Constants::Dirs::Up)));
	if (!((glm::angle(newOrientation, Constants::Dirs::Up) <= glm::radians(5.0f)) || (glm::angle(newOrientation, -Constants::Dirs::Up) <= glm::radians(5.0f)))) {
		this->transform->rotation = newOrientation;
	}
	this->transform->rotation = glm::rotate(this->transform->rotation, glm::radians(rotY), Constants::Dirs::Up);

	// Scroll changes speed
	if (display.scroll != 0) {
		display.scroll > 0 ? moveSpeed += 0.5f : (moveSpeed > 0.5f ? moveSpeed -= 0.5f : moveSpeed = 0.5f);
	}

	glfwSetCursorPos(window, centerX, centerY);
}