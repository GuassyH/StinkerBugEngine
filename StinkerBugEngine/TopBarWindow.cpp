#include "TopBarWindow.h"

void TopBarWindow::Init() {
	PlayTexture = Texture("assets/textures/editor", "PlayButton.png");
	PlayTexture.numColCh = 4;
	PlayTexture.Load(true, GL_LINEAR);

	PauseTexture = Texture("assets/textures/editor", "PauseButton.png");
	PauseTexture.numColCh = 4;
	PauseTexture.Load(true, GL_LINEAR);

	NextTexture = Texture("assets/textures/editor", "NextButton.png");
	NextTexture.numColCh = 4;
	NextTexture.Load(true, GL_LINEAR);
}

void TopBarWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {

	ImGui::SetNextWindowSize(ImVec2(display.windowWidth, 30));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Top_ToolbarWindow", &opened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDocking);
	ImGui::SetCursorPos(ImVec2(0, 0));
	ImGui::BeginChild("Top_Toolbar", ImVec2(ImGui::GetWindowSize().x, 30), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGuiStyle* custom_style = &ImGui::GetStyle();
	ImVec2 org_pad = custom_style->ItemSpacing;
	custom_style->ItemSpacing = ImVec2(0.0, 0.0);
	ImGui::SetCursorPos(ImVec2((display.windowWidth / 2.0f) - 30, 3));
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


	ImGui::EndChild();
	ImGui::End();
}