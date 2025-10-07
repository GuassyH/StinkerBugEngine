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

	topBarWindow.Init();
	sceneViewWindow.Init(Editor_ECS);

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	


	std::cout << "\nImGui / UI initialized\n\n";
}


void UI::imgui_render(Scene& scene) {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	int mode = 0;
	glfwGetInputMode(display.window, mode);

	// topBarWindow.Draw(scene, is_entity_selected, selected_entity, Editor_ECS);


	static bool opt_fullscreen = true;
	static bool dockspaceOpen = true;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	}
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Create the dockspace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	}


	gameWindow.Draw(scene, is_entity_selected, selected_entity, Editor_ECS);
	sceneViewWindow.Draw(scene, is_entity_selected, selected_entity, Editor_ECS);
	hierarchyWindow.Draw(scene, is_entity_selected, selected_entity, Editor_ECS);
	inspectorWindow.Draw(scene, is_entity_selected, selected_entity, Editor_ECS);
	consoleWindow.Draw(scene, is_entity_selected, selected_entity, Editor_ECS);

	// ImGui::ShowDemoWindow();
	ImGui::End();

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

