#include "HierarchyWindow.h"

#include "EntitySelector.h"
#include "ComponentsList.h"
#include "EntityObject.h"

/// THIS IS THE HIERARCHY
void HierarchyWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {

	ImGui::Begin("Hierarchy Menu");

	ImGui::Spacing();

	for (auto ID : scene.Scene_ECS.entities) {
		EntitySelector().Draw(scene, ID, is_entity_selected, selected_entity, editor_ecs);
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
	}

	double m_x, m_y;
	glfwGetCursorPos(display.window, &m_x, &m_y);

	ImGui::SetNextWindowSize(ImVec2(180, 300));
	ImGui::SetNextWindowPos(ImVec2((float)m_x, (float)m_y), ImGuiCond_Once); // simplified
	if (ImGui::BeginPopup("Create Object", ImGuiWindowFlags_NoMove)) {
		ImGui::Text("Create Object");
		if (ImGui::Button("Create Empty Entity", ImVec2(180, 20))) {
			EntityObject new_ntt = scene.CreateEntity();
			selected_entity = new_ntt.transform->entity;
			ImGui::CloseCurrentPopup();
		}

		ImGui::Separator();

		if (ImGui::Button("Create Directional Light", ImVec2(180, 20))) {
			EntityObject new_ntt = scene.CreateEntity();
			scene.Scene_ECS.entity_names[new_ntt.transform->entity] = "Light (" + std::to_string(new_ntt.transform->entity) + ")";
			new_ntt.transform->AddComponent<Light>();
			new_ntt.transform->GetComponent<Light>().light_type = LightTypes::Directional;
			new_ntt.transform->GetComponent<Transform>().rotation = glm::vec3(25.0f, 205.0f, 0.0f);
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Point Light", ImVec2(180, 20))) {
			EntityObject new_ntt = scene.CreateEntity();
			scene.Scene_ECS.entity_names[new_ntt.transform->entity] = "Point Light (" + std::to_string(new_ntt.transform->entity) + ")";
			new_ntt.transform->AddComponent<Light>();
			new_ntt.transform->GetComponent<Light>().light_type = LightTypes::Point;
			new_ntt.transform->GetComponent<Light>().radius_o = 10.0f;
			new_ntt.transform->GetComponent<Light>().radius_i = 0.5f;
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Camera", ImVec2(180, 20))) {
			EntityObject new_ntt = scene.CreateEntity();
			scene.Scene_ECS.entity_names[new_ntt.transform->entity] = "Camera (" + std::to_string(new_ntt.transform->entity) + ")";
			new_ntt.transform->AddComponent<Camera>(1920, 1080);
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Cube", ImVec2(180, 20))) {
			EntityObject new_ntt = scene.CreateEntity();
			scene.Scene_ECS.entity_names[new_ntt.transform->entity] = "Cube (" + std::to_string(new_ntt.transform->entity) + ")";
			new_ntt.transform->AddComponent<MeshRenderer>(std::make_shared<Model>(Constants::Shapes::Cube()), std::make_shared<Material>(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
			new_ntt.transform->GetComponent<MeshRenderer>().material->color = Constants::Colors::White;
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Sphere", ImVec2(180, 20))) {
			EntityObject new_ntt = scene.CreateEntity();
			scene.Scene_ECS.entity_names[new_ntt.transform->entity] = "Sphere (" + std::to_string(new_ntt.transform->entity) + ")";
			new_ntt.transform->AddComponent<MeshRenderer>(std::make_shared<Model>(Constants::Shapes::UVSphere()), std::make_shared<Material>(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
			new_ntt.transform->GetComponent<MeshRenderer>().material->color = Constants::Colors::White;
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Plane", ImVec2(180, 20))) {
			EntityObject new_ntt = scene.CreateEntity();
			scene.Scene_ECS.entity_names[new_ntt.transform->entity] = "Plane (" + std::to_string(new_ntt.transform->entity) + ")";
			new_ntt.transform->AddComponent<MeshRenderer>(std::make_shared<Model>(Constants::Shapes::Plane()), std::make_shared<Material>(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
			new_ntt.transform->GetComponent<MeshRenderer>().material->color = Constants::Colors::White;
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::Separator();

		if (ImGui::Button("Create Goblin", ImVec2(180, 20))) {
			EntityObject new_ntt = scene.CreateEntity();
			scene.Scene_ECS.entity_names[new_ntt.transform->entity] = "Goblin (" + std::to_string(new_ntt.transform->entity) + ")";
			new_ntt.transform->AddComponent<MeshRenderer>(std::make_shared<Model>(glm::vec3(0.05f)), std::make_shared<Material>(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
			new_ntt.transform->GetComponent<MeshRenderer>().model->loadModel("assets/models/lotr_troll/scene.gltf");
			new_ntt.transform->GetComponent<Transform>().scale = glm::vec3(0.05f);
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}


		ImGui::EndPopup();
	}

	ImGui::SetNextWindowSize(ImVec2(110, 80));
	ImGui::SetNextWindowPos(ImVec2((float)m_x, (float)m_y), ImGuiCond_Once); // simplified
	if (ImGui::BeginPopup("Change Object", ImGuiWindowFlags_NoMove)) {
		ImGui::Text("Change Object");
		ImGui::Separator();
		if (ImGui::Button("Duplicate", ImVec2(110, 20))) {
			scene.Scene_ECS.DuplicateEntity(selected_entity);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Delete", ImVec2(110, 20))) {
			scene.Scene_ECS.DestroyEntity(selected_entity);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}