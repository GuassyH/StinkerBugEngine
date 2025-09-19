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
	custom_style->Colors[ImGuiCol_WindowBg] = ImVec4(0.1, 0.1, 0.13, 1.0);
	custom_style->Colors[ImGuiCol_Header] = custom_style->Colors[ImGuiCol_Button];
	custom_style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.8, 0.8, 0.8, 1.0);
	custom_style->Colors[ImGuiCol_TitleBgActive] = custom_style->Colors[ImGuiCol_TitleBg];

	ImGui_ImplGlfw_InitForOpenGL(display.window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	r_windowWidth = display.windowWidth - 680;
	r_windowHeight = display.windowHeight - 340;

	std::cout << "\nImGui / UI initialized\n\n";
}


void UI::imgui_render(CameraMovement& camera_move, Scene& scene) {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	int mode = 0;
	glfwGetInputMode(display.window, mode);

	if (camera_move.focusMouse) { ImGui::SetMouseCursor(ImGuiMouseCursor_None); glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); }
	else { ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow); glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

	if (!camera_move.focusMouse) {
		HierarchyWindow().Draw(scene, display, is_entity_selected, selected_entity);
		InspectorWindow().Draw(scene, display, is_entity_selected, selected_entity);
		Console();
	}

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
	ImGui::SetNextWindowPos(ImVec2(350, display.windowHeight - 340));
	ImGui::SetNextWindowSize(ImVec2(display.windowWidth - 700, 340));
	ImGui::Begin("Console", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	const auto& lines = consoleCapture.GetLines();
	for (const auto& line : lines) {
		ImGui::TextUnformatted(line.c_str());
	}
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);


	ImGui::End();
}