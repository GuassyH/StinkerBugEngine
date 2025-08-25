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
		ImGui::Begin("Debug Menu", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		
		ImGui::DragFloat("Gravity", &scene.gravity, 0.1f, -50.0f, 50.0f);
		ImGui::Text("Entities");
		for (size_t i = 0; i < scene.Scene_ECS.entities.size(); i++)
		{
			std::ostringstream ss; ss << scene.Scene_ECS.entity_names[i];
			ImGui::DragFloat3(ss.str().c_str(), &scene.Scene_ECS.transforms[i].position.x, 0.1f, -1000, 1000);
		}
	
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

