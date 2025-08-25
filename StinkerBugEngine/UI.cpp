#include "UI.h"
#include "Camera.h"
#include "CameraMovement.h"
#include "Scene.h"


void UI::imgui_init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
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
		Hierarchy(camera_move, scene);
		EntityInspector(camera_move, scene);
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
	for (uint32_t i = 0; i < scene.Scene_ECS.entities.size(); i++)
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
void UI::EntityInspector(CameraMovement& camera_move, Scene& scene) {

	ImGui::SetNextWindowPos(ImVec2(display.windowWidth - 360, 10));
	ImGui::SetNextWindowSize(ImVec2(350, 1060));
	ImGui::Begin("Entity Inspector", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	std::ostringstream ss; ss << scene.Scene_ECS.entity_names[selected_entity];
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - ImGui::CalcTextSize(ss.str().c_str()).x * 0.5f);
	ImGui::Text(ss.str().c_str());

	ImGui::SetNextItemWidth(ImGui::CalcTextSize("XXXX XXXX XXXX XXXX").x);
	if (ImGui::InputText("NEW NAME", buff, sizeof(buff))) {
		scene.Scene_ECS.entity_names[selected_entity] = buff;
	}

	if (ImGui::CollapsingHeader("Transform")) {
		ImGui::DragFloat3("Position", &scene.Scene_ECS.transforms[selected_entity].position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &scene.Scene_ECS.transforms[selected_entity].rotation.x, 0.1f);
		ImGui::DragFloat3("Scale", &scene.Scene_ECS.transforms[selected_entity].scale.x, 0.1f);
	}
	if (scene.Scene_ECS.HasComponent<RigidBody>(selected_entity)) {
		if (ImGui::CollapsingHeader("RigidBody")) {
			ImGui::DragFloat("Mass", &scene.Scene_ECS.rigidbodies[selected_entity].mass, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Bounciness", &scene.Scene_ECS.rigidbodies[selected_entity].bounciness, 0.1f, 0.0f, 1);
			ImGui::DragFloat("Drag", &scene.Scene_ECS.rigidbodies[selected_entity].drag, 0.1f, 0.0f, 1);
		}
	}
	if (scene.Scene_ECS.HasComponent<Camera>(selected_entity)) {
		if (ImGui::CollapsingHeader("Camera")) {
			ImGui::DragFloat("FOV", &scene.Scene_ECS.cameras[selected_entity].FOVdeg, 0.1f, 0.0f, 120.0f);
			ImGui::DragFloat("Near Plane", &scene.Scene_ECS.cameras[selected_entity].nearPlane, 0.1f, 0.1f, 2000);
			ImGui::DragFloat("Far Plane", &scene.Scene_ECS.cameras[selected_entity].farPlane, 0.1f, 0.1f, 2000);
		}
	}
	if (scene.Scene_ECS.HasComponent<Light>(selected_entity)) {
		if (ImGui::CollapsingHeader("Light NOT WORKING")) {
			ImGui::ColorPicker4("Color", &scene.Scene_ECS.lights[selected_entity].color.r);
		}
	}


	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 125);
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);
	ImGui::Button("Add Component", ImVec2(250, 25));

	ImGui::End();
}

