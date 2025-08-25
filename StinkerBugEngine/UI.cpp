#include "UI.h"
#include "Camera.h"
#include "CameraMovement.h"
#include "Scene.h"


void UI::imgui_init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(display.window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	r_windowWidth = display.windowWidth - 680;
	r_windowHeight = display.windowHeight - 340;

	std::cout << "ImGui / UI initialized\n";
}

uint32_t selected_entity = 0;
void UI::imgui_render(CameraMovement& camera_move, Scene& scene) {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	int mode = 0;
	glfwGetInputMode(display.window, mode);

	if (camera_move.focusMouse) { ImGui::SetMouseCursor(ImGuiMouseCursor_None); glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); }
	else { ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow); glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

	if (!camera_move.focusMouse) {
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2(350, 1060));
		ImGui::Begin("Hierarchy Menu", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		
		ImGui::DragFloat("Gravity", &scene.gravity, 0.1f, -50.0f, 50.0f);
		ImGui::Text("Entities");
		for (uint32_t i = 0; i < scene.Scene_ECS.entities.size(); i++)
		{
			std::ostringstream ss; ss << scene.Scene_ECS.entity_names[i];
			if(ImGui::Button(ss.str().c_str(), ImVec2(330, 20))) {
				selected_entity = i;
				std::cout << "Selected entity " << selected_entity << "\n";
			}
		}
	
		ImGui::End();


		ImGui::SetNextWindowPos(ImVec2(display.windowWidth - 360, 10));
		ImGui::SetNextWindowSize(ImVec2(350, 1060));
		ImGui::Begin("GameObject Inspector", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		std::ostringstream ss; ss << scene.Scene_ECS.entity_names[selected_entity];
		ImGui::Text(ss.str().c_str());
		ImGui::DragFloat3("Transform", &scene.Scene_ECS.transforms[selected_entity].position.x, 0.1f);

		ImGui::End();
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

