#include "TopBarWindow.h"

void TopBarWindow::Init() {
	PlayTexture = Texture("editor/textures", "PlayButton.png");
	PlayTexture.numColCh = 4;
	PlayTexture.Load(true, GL_LINEAR);

	PauseTexture = Texture("editor/textures", "PauseButton.png");
	PauseTexture.numColCh = 4;
	PauseTexture.Load(true, GL_LINEAR);

	NextTexture = Texture("editor/textures", "NextButton.png");
	NextTexture.numColCh = 4;
	NextTexture.Load(true, GL_LINEAR);

	PlayingBlip = Texture("editor/textures", "PlayingBlip.png");
	PlayingBlip.numColCh = 4;
	PlayingBlip.Load(true, GL_LINEAR);

	PausedBlip = Texture("editor/textures", "PausedBlip.png");
	PausedBlip.numColCh = 4;
	PausedBlip.Load(true, GL_LINEAR);
}

void TopBarWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {

	ImGui::SetNextWindowSize(ImVec2(display.windowWidth, 30.0f));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Top_ToolbarWindow", &opened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDocking);

	ImGuiStyle* custom_style = &ImGui::GetStyle();
	ImVec2 org_pad = custom_style->ItemSpacing;
	custom_style->ItemSpacing = ImVec2(0.0, 0.0);

	if (play_scene) {
		ImGui::SetCursorPos(ImVec2((display.windowWidth / 2.0f) - 70, 7));
		ImGui::Image((ImTextureID)(intptr_t) pause_scene ? PausedBlip.ID : PlayingBlip.ID, ImVec2(20, 20));
	}

	ImGui::SetCursorPos(ImVec2((display.windowWidth / 2.0f) - 45, 3));
	if(ImGui::ImageButton("PlayButton", (ImTextureID)(intptr_t)PlayTexture.ID, ImVec2(20, 20), ImVec2(0,0), ImVec2(1,1))) {
		play_scene = play_scene ? false : true;
	}

	ImGui::SameLine();
	if (ImGui::ImageButton("PauseButton", (ImTextureID)(intptr_t)PauseTexture.ID, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1))) {
		pause_scene = pause_scene ? false : true;
	}

	ImGui::SameLine();
	if (ImGui::ImageButton("NextButton", (ImTextureID)(intptr_t)NextTexture.ID, ImVec2(20, 20))) {
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

	custom_style->ItemSpacing = org_pad;

	ImGui::End();
}