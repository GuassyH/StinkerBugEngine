#include "UI.h"
#include "Camera.h"
#include "CameraMovement.h"
#include "ComponentsList.h"
#include "Scene.h"
#include "Material.h"

#include "HierarchyWindow.h"
#include "InspectorWindow.h"
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


	
	sceneViewWindow.Draw(scene, is_entity_selected, selected_entity);
	topBarWindow.Draw(scene, is_entity_selected, selected_entity);
	HierarchyWindow().Draw(scene, is_entity_selected, selected_entity);
	InspectorWindow().Draw(scene, is_entity_selected, selected_entity);
	Console();

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




/// CONSOLE OUTPUT
void UI::Console() {
	bool opened = true;
	ImGui::SetNextWindowPos(ImVec2(350, display.windowHeight - 300));
	ImGui::SetNextWindowSize(ImVec2(display.windowWidth - 700, 300));
	ImGui::Begin("Console", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	const auto& lines = consoleCapture.GetLines();
	for (const auto& line : lines) {
		ImGui::TextUnformatted(line.c_str());
	}
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);


	ImGui::End();
}