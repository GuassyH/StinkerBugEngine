#include "UI.h"
#include "Camera.h"
#include "CameraMovement.h"
#include "ComponentsList.h"
#include "Scene.h"
#include "Material.h"

void UI::imgui_init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(display.window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	r_windowWidth = display.windowWidth - 680;
	r_windowHeight = display.windowHeight - 340;

	std::cout << "ImGui / UI initialized\n\n";
}



void UI::imgui_render(CameraMovement& camera_move, Scene& scene, Mesh& cube, Material& mat) {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	int mode = 0;
	glfwGetInputMode(display.window, mode);

	if (camera_move.focusMouse) { ImGui::SetMouseCursor(ImGuiMouseCursor_None); glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); }
	else { ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow); glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

	if (!camera_move.focusMouse) {
		Hierarchy(camera_move, scene);
		EntityInspector(camera_move, scene, cube, mat);
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



void UI::Hierarchy(CameraMovement& camera_move, Scene& scene) {
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::SetNextWindowSize(ImVec2(350, 1060));
	ImGui::Begin("Hierarchy Menu", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	ImGui::DragFloat("Gravity", &scene.gravity, 0.1f, -50.0f, 50.0f);
	ImGui::Text("Entities");
	for (uint32_t i = 0; i < scene.Scene_ECS.entity_names.size(); i++)
	{
		std::ostringstream ss; ss << scene.Scene_ECS.entity_names[i];
		if (ImGui::Button(ss.str().c_str(), ImVec2(330, 20))) {
			selected_entity = i;
			std::cout << "Selected entity " << selected_entity << "\n";
		}
	}


	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 125);
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);
	if (ImGui::Button("Add Entity", ImVec2(250, 25))) {
		scene.CreateEntity();
		std::cout << "Created new entity\n";
	}

	ImGui::End();

}


char buff[255];
char* new_name;
void UI::EntityInspector(CameraMovement& camera_move, Scene& scene, Mesh& cube, Material& mat) {

	ImGui::SetNextWindowPos(ImVec2(display.windowWidth - 360, 10));
	ImGui::SetNextWindowSize(ImVec2(350, 1060));
	ImGui::Begin("Entity Inspector", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	std::ostringstream ss; ss << scene.Scene_ECS.entity_names[selected_entity];
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - ImGui::CalcTextSize(ss.str().c_str()).x * 0.5f);

	
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 180 * 0.5f);
	ImGui::SetNextItemWidth(180);
	if (ImGui::InputTextWithHint(" ", ss.str().c_str(), buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll)) {
		scene.Scene_ECS.entity_names[selected_entity] = buff;
		memset(buff, 0, 255);
	}

	scene.Scene_ECS.GetComponent<Transform>(selected_entity).DrawInInspector();

	if (scene.Scene_ECS.HasComponent<RigidBody>(selected_entity)) {
		scene.Scene_ECS.GetComponent<RigidBody>(selected_entity).DrawInInspector();
	}
	if (scene.Scene_ECS.HasComponent<Camera>(selected_entity)) {
		scene.Scene_ECS.GetComponent<Camera>(selected_entity).DrawInInspector();
	}
	if (scene.Scene_ECS.HasComponent<Light>(selected_entity)) {
		scene.Scene_ECS.GetComponent<Light>(selected_entity).DrawInInspector();
	}
	if (scene.Scene_ECS.HasComponent<MeshRenderer>(selected_entity)) {
		scene.Scene_ECS.GetComponent<MeshRenderer>(selected_entity).DrawInInspector();
	}


	// Center button
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 125);
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);

	if (ImGui::Button("Add Component", ImVec2(250, 25))) {
		ImGui::OpenPopup("Add Component");
	}

	// Setup popup
	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(display.windowWidth - 285, display.windowHeight-360), ImGuiCond_Once); // simplified

	if (ImGui::BeginPopup("AddComponent")) {
		ImGui::Text("Add Component");
		ImGui::Separator();
		ImGui::Text("Select a component to add:");
		ImGui::Separator();

		// I mean it works but its not efficient. Should be a loop for each component type add component
		if (!scene.Scene_ECS.HasComponent<MeshRenderer>(selected_entity)) {
			if (ImGui::Button("Mesh Renderer", ImVec2(180, 20))) {
				scene.Scene_ECS.AddComponent<MeshRenderer>(selected_entity, cube, mat);
				ImGui::CloseCurrentPopup();
			}
		}
		if (!scene.Scene_ECS.HasComponent<RigidBody>(selected_entity)) {
			if (ImGui::Button("Rigidbody", ImVec2(180, 20))) {
				scene.Scene_ECS.AddComponent<RigidBody>(selected_entity);
				ImGui::CloseCurrentPopup();
			}
		}


		ImGui::EndPopup();
	}

	ImGui::End();
}