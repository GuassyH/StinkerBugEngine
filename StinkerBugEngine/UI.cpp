#include "UI.h"
#include "Camera.h"
#include "ComponentsList.h"
#include "Scene.h"
#include "Material.h"

/// CORE

void UI::imgui_init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsClassic();
	
	float bright = 1.5f;

	ImGuiStyle* custom_style = &ImGui::GetStyle();
	custom_style->Colors[ImGuiCol_Header] = custom_style->Colors[ImGuiCol_Button];
	custom_style->Colors[ImGuiCol_TitleBgActive] = custom_style->Colors[ImGuiCol_TitleBg];
	custom_style->Colors[ImGuiCol_WindowBg] = ImVec4(0.1f * bright, 0.1f * bright, 0.13f * bright, 1.0f);
	custom_style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	custom_style->Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.26f, 1.0f);


	custom_style->WindowPadding = ImVec2(1.0, 1.0);
	custom_style->ItemSpacing = ImVec2(10, 5);

	ImGui_ImplGlfw_InitForOpenGL(display.window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	topBarWindow.Init();
	sceneViewWindow.Init(Editor_ECS);

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingAlwaysTabBar = true;


	std::cout << "\nImGui / UI initialized\n\n";
}


void UI::imgui_render(Scene* scene) {
	if (!scene) { std::runtime_error("ImGui tried to render with a null scene"); }

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	int mode = 0;
	glfwGetInputMode(display.window, mode);

	topBarWindow.Draw(*scene, is_entity_selected, selected_entity, Editor_ECS);


	static bool opt_fullscreen = true;
	static bool dockspaceOpen = true;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		// Temp size adjustment to account for top bar
		ImVec2 workPos = viewport->WorkPos; workPos.y += 13;
		ImVec2 workSize = viewport->WorkSize; workSize.y -= 13;
		ImGui::SetNextWindowPos(workPos);
		ImGui::SetNextWindowSize(workSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

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


	gameWindow.Draw(*scene, is_entity_selected, selected_entity, Editor_ECS);
	sceneViewWindow.Draw(*scene, is_entity_selected, selected_entity, Editor_ECS);
	hierarchyWindow.Draw(*scene, is_entity_selected, selected_entity, Editor_ECS);
	inspectorWindow.Draw(*scene, is_entity_selected, selected_entity, Editor_ECS);
	consoleWindow.Draw(*scene, is_entity_selected, selected_entity, Editor_ECS);
	assetWindow.Draw();

	ImGui::End();

	glDisable(GL_FRAMEBUFFER_SRGB);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glEnable(GL_FRAMEBUFFER_SRGB);

	
	// io.IniFilename = "layout.ini"; // This like resets it? Im unsure
}


void UI::imgui_shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	std::cout << "UI shutdown\n";
}

/// END OF CORE

