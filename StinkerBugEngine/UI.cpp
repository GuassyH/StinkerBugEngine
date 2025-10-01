#include "UI.h"
#include "Camera.h"
#include "CameraMovement.h"
#include "ComponentsList.h"
#include "Scene.h"
#include "Material.h"

/// CORE

void UI::imgui_init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsClassic();
	
	ImGuiStyle* custom_style = &ImGui::GetStyle();
	custom_style->Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.13f, 1.0f);
	custom_style->Colors[ImGuiCol_Header] = custom_style->Colors[ImGuiCol_Button];
	custom_style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	custom_style->Colors[ImGuiCol_TitleBgActive] = custom_style->Colors[ImGuiCol_TitleBg];
	custom_style->Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.26f, 1.0f);

	ImGui_ImplGlfw_InitForOpenGL(display.window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	sceneViewWindow.Init();

	std::cout << "\nImGui / UI initialized\n\n";
}


void UI::imgui_render(Scene& scene) {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	int mode = 0;
	glfwGetInputMode(display.window, mode);


	
	gameWindow.Draw(scene, is_entity_selected, selected_entity);
	sceneViewWindow.Draw(scene, is_entity_selected, selected_entity);
	topBarWindow.Draw(scene, is_entity_selected, selected_entity);
	hierarchyWindow.Draw(scene, is_entity_selected, selected_entity);
	inspectorWindow.Draw(scene, is_entity_selected, selected_entity);
	consoleWindow.Draw(scene, is_entity_selected, selected_entity);

	// ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void UI::imgui_shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	std::cout << "UI shutdown\n";
}

/// END OF CORE

