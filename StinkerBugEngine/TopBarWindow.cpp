#include "TopBarWindow.h"



void TopBarWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {

	ImGui::SetNextWindowSize(ImVec2(display.windowWidth, 30));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Top_ToolbarWindow", &opened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
	ImGui::SetCursorPos(ImVec2(0, 0));
	ImGui::BeginChild("Top_Toolbar", ImVec2(ImGui::GetWindowSize().x, 30), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::SetCursorPos(ImVec2((display.windowWidth / 2.0f) - 45, 5));
	ImGui::Selectable(">", &play_scene, ImGuiSelectableFlags_None, ImVec2(30, 30));
	ImGui::SameLine();
	ImGui::Selectable("||", &pause_scene, ImGuiSelectableFlags_None, ImVec2(30, 30));
	ImGui::SameLine(); 
	if (ImGui::Selectable(">++", &next_frame, ImGuiSelectableFlags_None, ImVec2(30, 30))) {
		if (play_scene) {
			if (!scene_started) {
				scene.StartEntityBehaviours();
				scene_started = true;
			}
			scene.UpdateEntityBehaviours();
			scene.UpdatePhysics();
		}
		next_frame = false;
	}

	// When playing scene the scene should be copied and played so that when you change something
	//	- it will be reset when you stop playing
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

	ImGui::EndChild();
	ImGui::End();
}