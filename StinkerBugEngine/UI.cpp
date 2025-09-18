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
		Hierarchy(scene);
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

/// END OF CORE


/// THIS IS THE HIERARCHY
void UI::Hierarchy(Scene& scene) {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(350, display.windowHeight));
	ImGui::Begin("Hierarchy Menu", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	ImGui::DragFloat("Gravity", &scene.gravity, 0.1f, -50.0f, 50.0f);
	ImGui::Separator();

	ImGui::Text("Entities");


	for (auto [id, name] : scene.Scene_ECS.entity_names) {
		std::ostringstream ss; ss << name;
		if (ImGui::Button(ss.str().c_str(), ImVec2(330, 20))) {
			selected_entity = id;
			is_entity_selected = true;
			std::cout << "Selected: " << name << "\n";
		}
	}

	if (glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && ImGui::IsWindowHovered()) {
		if (!ImGui::IsAnyItemHovered()) {
			is_entity_selected = false;
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
			EntityHelper new_ntt(scene.CreateEntity(), &scene.Scene_ECS);
			new_ntt.AddComponent<MeshRenderer>().mesh = new Mesh(Constants::Shapes::Cube());
			new_ntt.GetComponent<MeshRenderer>().material = new Material();
			new_ntt.GetComponent<MeshRenderer>().material->Color = Constants::Colors::White;
			new_ntt.~EntityHelper();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Sphere", ImVec2(180, 20))) {
			EntityHelper new_ntt(scene.CreateEntity(), &scene.Scene_ECS);
			new_ntt.AddComponent<MeshRenderer>().mesh = new Mesh(Constants::Shapes::UVSphere());
			new_ntt.GetComponent<MeshRenderer>().material = new Material();
			new_ntt.GetComponent<MeshRenderer>().material->Color = Constants::Colors::White;
			new_ntt.~EntityHelper();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Plane", ImVec2(180, 20))) {
			EntityHelper new_ntt(scene.CreateEntity(), &scene.Scene_ECS);
			new_ntt.AddComponent<MeshRenderer>().mesh = new Mesh(Constants::Shapes::Plane());
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

	if (scene.Scene_ECS.entities.find(selected_entity) != scene.Scene_ECS.entities.end() && is_entity_selected) {

	std::ostringstream ss; ss << scene.Scene_ECS.entity_names.find(selected_entity)->second;
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - ImGui::CalcTextSize(ss.str().c_str()).x * 0.5f);
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 180 * 0.5f);
	ImGui::SetNextItemWidth(180);
	
	if (ImGui::InputTextWithHint(" ", ss.str().c_str(), buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll)) {
		scene.Scene_ECS.entity_names[selected_entity] = buff;
		memset(buff, 0, 255);
	}
	ImGui::SameLine();
	if (ImGui::Button("U")) {	// You can now delete entities but adding new ones after deleting doesnt work
		scene.Scene_ECS.DestroyEntity(selected_entity);
		is_entity_selected = false;
		goto BreakInspector;
	}

	
	// Foreach type, get all the components and draw in inspector
	// Collapsing component in one entity collapses the same component in ALL entities
	// Probably because the CollapsingHeader name is the same so ID is the same
	scene.Scene_ECS.GetComponent<Transform>(selected_entity).DrawOnInspector();
	ImGui::Separator();
	for (auto& [type, map] : scene.Scene_ECS.components){
		auto compPtr = map.find(selected_entity);
		if (compPtr != map.end() && compPtr->second && type != typeid(Transform)) { 
			Component* c = dynamic_cast<Component*>(compPtr->second.get());
			c->DrawOnInspector();
			ImGui::Separator();
		}
	}
	if (scene.Scene_ECS.HasComponent<Collider>(selected_entity)) 
	{ scene.Scene_ECS.GetComponent<Collider>(selected_entity).DrawOnInspector(); ImGui::Separator(); }



	// Center button
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 125);
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);

	if (ImGui::Button("Add Component", ImVec2(250, 25))) {
		ImGui::OpenPopup("Add Component");
	}

	// Setup popup

	ImGui::SetNextWindowPos(ImVec2(display.windowWidth - 300, display.windowHeight-360)); // simplified
	ImGui::SetNextWindowSize(ImVec2(250, 300));
	if (ImGui::BeginPopup("Add Component", ImGuiWindowFlags_NoMove)) {
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - ImGui::CalcTextSize("Add Component").x * 0.5f);
		ImGui::Text("Add Component");
		ImGui::Separator();
		ImGui::Text("Select a component to add:");
		ImGui::Separator();


		// I mean it works but its not efficient. Should be a loop for each component type add component like with the DrawOnInspector
		if (!scene.Scene_ECS.HasComponent<MeshRenderer>(selected_entity)) {
			if (ImGui::Button("Mesh Renderer", ImVec2(235, 20))) {
				EntityHelper new_ntt(selected_entity, & scene.Scene_ECS);
				new_ntt.AddComponent<MeshRenderer>().mesh = new Mesh(Constants::Shapes::Cube());
				new_ntt.GetComponent<MeshRenderer>().material = new Material();
				new_ntt.GetComponent<MeshRenderer>().material->Color = Constants::Colors::White;
				ImGui::CloseCurrentPopup();
				new_ntt.~EntityHelper();
			}
		}
		if (!scene.Scene_ECS.HasComponent<RigidBody>(selected_entity)) {
			if (ImGui::Button("Rigidbody", ImVec2(235, 20))) {
				scene.Scene_ECS.AddComponent<RigidBody>(selected_entity);
				ImGui::CloseCurrentPopup();
			}
		}
		if (!scene.Scene_ECS.HasComponent<TestComponent>(selected_entity)) {
			if (ImGui::Button("TestComponent", ImVec2(235, 20))) {
				scene.Scene_ECS.AddComponent<TestComponent>(selected_entity);
				ImGui::CloseCurrentPopup();
			}
		}

		if (!scene.Scene_ECS.HasComponent<BoxCollider>(selected_entity) && !scene.Scene_ECS.HasComponent<Collider>(selected_entity)) {
			if (ImGui::Button("BoxCollider", ImVec2(235, 20))) {
				scene.Scene_ECS.AddComponent<BoxCollider>(selected_entity);
				ImGui::CloseCurrentPopup();
			}
		}

		if (!scene.Scene_ECS.HasComponent<SphereCollider>(selected_entity) && !scene.Scene_ECS.HasComponent<Collider>(selected_entity)) {
			if (ImGui::Button("SphereCollider", ImVec2(235, 20))) {
				scene.Scene_ECS.AddComponent<SphereCollider>(selected_entity);
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}


	}

BreakInspector:
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