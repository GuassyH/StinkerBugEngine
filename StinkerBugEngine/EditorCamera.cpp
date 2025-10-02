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
	camera->nearPlane = 0.01f;
	camera->CheckOuputFBO(true);

	Screen::InitFBO(camera, select_fbo, select_rbo, select_tex);
}

void EditorCamera::AddGizmoEntities(Scene& scene, ECSystem& editor_ecs) {
	Gizmos::Gizmo infinite_grid = Gizmos::InfiniteGrid(editor_ecs);

	pre_pass_gizmos.push_back(infinite_grid);


	Gizmos::Gizmo transform_gizmo = Gizmos::TransformHandle(editor_ecs);
	Gizmos::Gizmo scale_gizmo = Gizmos::ScaleHandle(editor_ecs);
	Gizmos::Gizmo rotate_gizmo = Gizmos::RotateHandle(editor_ecs);

	post_pass_gizmos.push_back(transform_gizmo);
	post_pass_gizmos.push_back(scale_gizmo);
	post_pass_gizmos.push_back(rotate_gizmo);
}


void EditorCamera::PrePass(Scene& scene, ECSystem& editor_ecs) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	// Rebind the framebuffer to the editor camera's FBO
	glBindFramebuffer(GL_FRAMEBUFFER, camera->outputFBO);

	// DUMB SHOULD BE PER GIZMO WHICH SHOULD HAVE A "SHOW"
	if (showGrid) {
		for (auto& p_gizmo : pre_pass_gizmos) {
			p_gizmo.Draw(camera, scene, transform, true);
		}
	}

	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_BLEND);
}

void EditorCamera::Render(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {
	camera->Render(&scene);
	PrePass(scene, editor_ecs);
	PostPass(scene, is_entity_selected, selected_entity, editor_ecs);
	SelectObject(scene, is_entity_selected, selected_entity, editor_ecs);
}

void EditorCamera::PostPass(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Rebind the framebuffer to the editor camera's FBO
	glBindFramebuffer(GL_FRAMEBUFFER, camera->outputFBO);
	
	selected_entity_helper.ecs = &scene.Scene_ECS;
	selected_entity_helper.id = selected_entity;

	// If there isnt an entity selected then skip drawing gizmos, reset framebuffer 
	if (!is_entity_selected || scene.Scene_ECS.entities.find(selected_entity) == scene.Scene_ECS.entities.end()) { glDisable(GL_BLEND); glBindFramebuffer(GL_FRAMEBUFFER, 0); return; }

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	if (glfwGetMouseButton(Display::getInstance().window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
		if (glfwGetKey(Display::getInstance().window, GLFW_KEY_W) == GLFW_PRESS) { selected_gizmo = 0; }
		if (glfwGetKey(Display::getInstance().window, GLFW_KEY_S) == GLFW_PRESS) { selected_gizmo = 1; }
		if (glfwGetKey(Display::getInstance().window, GLFW_KEY_R) == GLFW_PRESS) { selected_gizmo = 2; }
	}
	post_pass_gizmos[selected_gizmo].Draw(camera, scene, transform, selected_entity_helper, true);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_BLEND);
}

void EditorCamera::SelectObject(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {
	
	rayHit.hit = false;
	rayHit.isGizmo = false;

	// If you left click
	if (is_entity_selected) {
		// Gizmo only raycast
		rayHit = Physics::Raycast(Screen::ScreenToWorldRay(w_pos, w_size, camera), camera->farPlane, camera, Screen::GetMouseNDC(w_pos, w_size, glm::vec2(camera->width, camera->height)), editor_ecs);
	}

	if (glfwGetMouseButton(Display::getInstance().window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && 
		glfwGetMouseButton(Display::getInstance().window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) { 
		if (!rayHit.isGizmo) {
			if (firstClick) {
				firstClick = false;
				if (Screen::IsMouseInRect(w_pos, w_size)) {
					// Cast a ray from the mouse position
					Screen::ScreenCastHit scHit = Screen::EntityAtMousePos
					(camera, scene, Screen::GetMousePosInViewport(w_pos, w_size, glm::vec2(camera->width, camera->height)), select_fbo, select_rbo, select_tex);
				
					if (scHit.hit) {
						selected_entity = scHit.entity;
						is_entity_selected = true;
					}
					else {
						is_entity_selected = false;
					}
				}
			}
		}
		else {
			if (firstClick) {
				// Do Gizmo stuff
				EntityHelper gizmo(rayHit.entity, &editor_ecs);
				gizmo.GetComponent<GizmoComponent>().isHovered = true;
				firstClick = false;
				interactingWithGizmo = true;
			}
		}
	}
	else { interactingWithGizmo = false; firstClick = true; }

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