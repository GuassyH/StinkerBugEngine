#include "EditorCamera.h"
#include "Scene.h"
#include "Screen.h"

void EditorCamera::AddGizmoEntities(Scene& scene) {
	Gizmos::Gizmo transform_gizmo = Gizmos::TransformHandle();
	Gizmos::Gizmo scale_gizmo = Gizmos::ScaleHandle();
	Gizmos::Gizmo rotate_gizmo = Gizmos::RotateHandle();

	gizmos.push_back(transform_gizmo);
	gizmos.push_back(scale_gizmo);
	gizmos.push_back(rotate_gizmo);
}

void EditorCamera::DrawGizmos(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {
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
				Screen::ScreenCastHit scHit = Screen::EntityAtMousePos(camera, scene, Screen::GetMousePosInViewport(w_pos, w_size, glm::vec2(camera->width, camera->height)));

				if (scHit.hit) {
					selected_entity = scHit.entity;
					is_entity_selected = true;
					std::cout << "Mouse hit entity: " << scHit.entity << "\n";
				}
				else {
					is_entity_selected = false;
					std::cout << "Mouse did not hit any entity\n";
				}
			}
		}
	}
	else {
		firstClick = true;
	}
}