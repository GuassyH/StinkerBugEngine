#include "TopBarWindow.h"


void TopBarWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {

	ImGui::SetNextWindowSize(ImVec2(display.windowWidth, 40));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Scene Manipulator", &opened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	ImGui::SetCursorPos(ImVec2((display.windowWidth / 2.0f) - 150, 5));
	ImGui::Selectable("Play", &play_scene, ImGuiSelectableFlags_None, ImVec2(50, 30));
	ImGui::SameLine();
	ImGui::Selectable("Pause", &pause_scene, ImGuiSelectableFlags_None, ImVec2(50, 30));
	ImGui::SameLine();
	if (ImGui::Button("Next", ImVec2(50, 30))) {
		if (play_scene) {
			if (!scene_started) {
				scene.StartEntityBehaviours();
				scene_started = true;
			}
			scene.UpdateEntityBehaviours();
			scene.UpdatePhysics();
		}
	}


	// When playing scene the scene should be copied and played so that when you change something
	// It will be reset when you stop playing
	if (play_scene) {
		if (!pause_scene) {
			if (!scene_started) {
				scene.StartEntityBehaviours();
				scene_started = true;
			}
			scene.UpdateEntityBehaviours();
			scene.UpdatePhysics();
		}
	}
	else {
		scene_started = false;
	}

	ImGui::End();
}