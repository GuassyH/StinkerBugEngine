#include "UI.h"
#include "Camera.h"
#include "CameraMovement.h"
#include "ComponentsList.h"
#include "Scene.h"
#include "Material.h"

/// INITIALIZATION

void UI::imgui_init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
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
		Hierarchy(camera_move, scene);
		EntityInspector(scene);
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





/// THIS IS THE HIERARCHY
void UI::Hierarchy(CameraMovement& camera_move, Scene& scene) {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(350, display.windowHeight));
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


	if (glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS && ImGui::IsWindowHovered()) {
		if (!ImGui::IsAnyItemHovered()) {
			ImGui::OpenPopup("Create Object");
		}
		else if (ImGui::IsAnyItemHovered()) {
			ImGui::OpenPopup("Change Object");
		}
	}

	double m_x, m_y;
	glfwGetCursorPos(display.window, &m_x, &m_y);

	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(m_x, m_y), ImGuiCond_Once); // simplified
	if (ImGui::BeginPopup("Create Object")) {
		ImGui::Text("Create Object");
		if (ImGui::Button("Create Empty Entity", ImVec2(180, 20))) {
			scene.CreateEntity();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Cube", ImVec2(180, 20))) {
			EntityHelper new_ntt(scene.CreateEntity("Cube"), &scene.Scene_ECS);
			new_ntt.AddComponent<MeshRenderer>().mesh = new Mesh(Constants::Shapes::Cube());
			new_ntt.GetComponent<MeshRenderer>().material = new Material();
			new_ntt.GetComponent<MeshRenderer>().material->Color = Constants::Colors::White;
			new_ntt.~EntityHelper();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Sphere", ImVec2(180, 20))) {
			EntityHelper new_ntt(scene.CreateEntity("Sphere"), &scene.Scene_ECS);
			new_ntt.AddComponent<MeshRenderer>().mesh = new Mesh(Constants::Shapes::UVSphere());
			new_ntt.GetComponent<MeshRenderer>().material = new Material();
			new_ntt.GetComponent<MeshRenderer>().material->Color = Constants::Colors::White;
			new_ntt.~EntityHelper();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}


	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(m_x, m_y), ImGuiCond_Once); // simplified
	if (ImGui::BeginPopup("Change Object")) {
		ImGui::Text("Change Object");
		if (ImGui::Button("Delete Entity", ImVec2(180, 20))) {
			// scene.DeleteEntity();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::End();

}




/// THIS IS THE ENTITY INSPECTOR
char buff[255];
char* new_name;
void UI::EntityInspector(Scene& scene) {

	ImGui::SetNextWindowPos(ImVec2(display.windowWidth - 350, 0));
	ImGui::SetNextWindowSize(ImVec2(350, display.windowHeight));
	ImGui::Begin("Entity Inspector", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	std::ostringstream ss; ss << scene.Scene_ECS.entity_names[selected_entity];
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - ImGui::CalcTextSize(ss.str().c_str()).x * 0.5f);
	
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 180 * 0.5f);
	ImGui::SetNextItemWidth(180);
	if (ImGui::InputTextWithHint(" ", ss.str().c_str(), buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll)) {
		scene.Scene_ECS.entity_names[selected_entity] = buff;
		memset(buff, 0, 255);
	}


	// Foreach type, get all the components and draw in inspector
	// WORKS!!!
	scene.Scene_ECS.GetComponent<Transform>(selected_entity).DrawOnInspector();
	for (auto& [type, map] : scene.Scene_ECS.components){
		auto compPtr = map.find(selected_entity);
		if (compPtr != map.end() && compPtr->second && type != typeid(Transform)) { 
			Component* c = dynamic_cast<Component*>(compPtr->second.get());
			c->DrawOnInspector();
		}
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
	if (ImGui::BeginPopup("Add Component")) {
		ImGui::Text("Add Component");
		ImGui::Separator();
		ImGui::Text("Select a component to add:");
		ImGui::Separator();


		// I mean it works but its not efficient. Should be a loop for each component type add component like with the DrawOnInspector
		if (!scene.Scene_ECS.HasComponent<MeshRenderer>(selected_entity)) {
			if (ImGui::Button("Mesh Renderer", ImVec2(180, 20))) {
				EntityHelper new_ntt(selected_entity, & scene.Scene_ECS);
				new_ntt.AddComponent<MeshRenderer>().mesh = new Mesh(Constants::Shapes::Cube());
				new_ntt.GetComponent<MeshRenderer>().material = new Material();
				new_ntt.GetComponent<MeshRenderer>().material->Color = Constants::Colors::White;
				ImGui::CloseCurrentPopup();
				new_ntt.~EntityHelper();
			}
		}
		if (!scene.Scene_ECS.HasComponent<RigidBody>(selected_entity)) {
			if (ImGui::Button("Rigidbody", ImVec2(180, 20))) {
				scene.Scene_ECS.AddComponent<RigidBody>(selected_entity);
				ImGui::CloseCurrentPopup();
			}
		}
		if (!scene.Scene_ECS.HasComponent<TestComponent>(selected_entity)) {
			if (ImGui::Button("TestComponent", ImVec2(180, 20))) {
				scene.Scene_ECS.AddComponent<TestComponent>(selected_entity);
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}



	ImGui::End();
}






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