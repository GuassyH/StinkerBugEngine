#include "HierarchyWindow.h"

#include <string.h>

#include "ComponentsList.h"
#include "EntityHelper.h"

/// THIS IS THE HIERARCHY
void HierarchyWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {
	ImGui::SetNextWindowPos(ImVec2(0, 40));
	ImGui::SetNextWindowSize(ImVec2(350, display.windowHeight - 40));
	ImGui::Begin("Hierarchy Menu", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	ImGui::DragFloat("Gravity", &scene.gravity, 0.1f);
	ImGui::Separator();

	ImGui::Text("Entities");
	for (auto [id, name] : scene.Scene_ECS.entity_names) {
		std::ostringstream ss; ss << name;
		// Should maybe be a selectable instead?
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
	ImGui::SetNextWindowPos(ImVec2((float)m_x, (float)m_y), ImGuiCond_Once); // simplified
	if (ImGui::BeginPopup("Create Object")) {
		ImGui::Text("Create Object");
		if (ImGui::Button("Create Empty Entity", ImVec2(180, 20))) {
			selected_entity = scene.CreateEntity();
			ImGui::CloseCurrentPopup();
		}

		ImGui::Separator();

		if (ImGui::Button("Create Directional Light", ImVec2(180, 20))) {
			EntityHelper new_ntt(scene.CreateEntity(), &scene.Scene_ECS);
			new_ntt.AddComponent<Light>();
			new_ntt.GetComponent<Light>().light_type = LightTypes::Directional;
			new_ntt.GetComponent<Transform>().rotation = glm::vec3(-55.0f, 15.0f, 0.0f);
			selected_entity = new_ntt.id;
			new_ntt.~EntityHelper();
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Create Camera", ImVec2(180, 20))) {
			EntityHelper new_ntt(scene.CreateEntity(), &scene.Scene_ECS);
			scene.Scene_ECS.entity_names[new_ntt.id] = "Camera (" + std::to_string(new_ntt.id) + ")";
			new_ntt.AddComponent<Camera>(1920, 1080, new_ntt.GetComponent<Transform>());
			selected_entity = new_ntt.id;
			new_ntt.~EntityHelper();
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Create Cube", ImVec2(180, 20))) {
			EntityHelper new_ntt(scene.CreateEntity(), &scene.Scene_ECS);
			scene.Scene_ECS.entity_names[new_ntt.id] = "Cube (" + std::to_string(new_ntt.id) + ")";
			Mesh n_m = Mesh(Constants::Shapes::Cube());
			new_ntt.AddComponent<MeshRenderer>().model = new Model(n_m);
			new_ntt.GetComponent<MeshRenderer>().material = new Material();
			new_ntt.GetComponent<MeshRenderer>().material->Color = Constants::Colors::White;
			selected_entity = new_ntt.id;
			new_ntt.~EntityHelper();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Sphere", ImVec2(180, 20))) {
			EntityHelper new_ntt(scene.CreateEntity(), &scene.Scene_ECS);
			scene.Scene_ECS.entity_names[new_ntt.id] = "Sphere (" + std::to_string(new_ntt.id) + ")";
			Mesh n_m = Mesh(Constants::Shapes::UVSphere());
			new_ntt.AddComponent<MeshRenderer>().model = new Model(n_m);
			new_ntt.GetComponent<MeshRenderer>().material = new Material();
			new_ntt.GetComponent<MeshRenderer>().material->Color = Constants::Colors::White;
			selected_entity = new_ntt.id;
			new_ntt.~EntityHelper();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Plane", ImVec2(180, 20))) {
			EntityHelper new_ntt(scene.CreateEntity(), &scene.Scene_ECS);
			scene.Scene_ECS.entity_names[new_ntt.id] = "Plane (" + std::to_string(new_ntt.id) + ")";
			Mesh n_m = Mesh(Constants::Shapes::Plane());
			new_ntt.AddComponent<MeshRenderer>().model = new Model(n_m);
			new_ntt.GetComponent<MeshRenderer>().material = new Material();
			new_ntt.GetComponent<MeshRenderer>().material->Color = Constants::Colors::White;
			selected_entity = new_ntt.id;
			new_ntt.~EntityHelper();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}


	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2((float)m_x, (float)m_y), ImGuiCond_Once); // simplified
	if (ImGui::BeginPopup("Change Object")) {
		ImGui::Text("Change Object");
		ImGui::EndPopup();
	}

	ImGui::End();
}