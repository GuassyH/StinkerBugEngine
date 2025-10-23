#include "HierarchyWindow.h"

#include "EntitySelector.h"
#include "ComponentsList.h"
#include "EntityObject.h"

/// THIS IS THE HIERARCHY
void HierarchyWindow::Draw(Scene* scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {

	ImGui::Begin("Hierarchy Menu");

	if (!scene) { ImGui::End(); return; }

	ImGui::Spacing();

	for (auto ID : scene->Scene_ECS.entities) {
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

	// ImGui::SetNextWindowSize(ImVec2(180, 300));
	ImGui::SetNextWindowPos(ImVec2((float)m_x, (float)m_y), ImGuiCond_Once); // simplified
	if (ImGui::BeginPopup("Create Object", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGuiStyle* custom_style = &ImGui::GetStyle();
		ImVec2 org_space = custom_style->ItemSpacing;
		custom_style->ItemSpacing = ImVec2(10, 1);

		ImVec2 button_size = ImVec2(150, 20);
		ImGui::Text("Create Object");
		if (ImGui::Button("Create Empty Entity", button_size)) {
			EntityObject new_ntt = scene->CreateEntity();
			selected_entity = new_ntt.transform->entity;
			ImGui::CloseCurrentPopup();
		}

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();

		if (ImGui::Button("Create Dir Light", button_size)) {
			EntityObject new_ntt = scene->CreateEntity("Directional Light");
			new_ntt.transform->AddComponent<Light>();
			new_ntt.transform->GetComponent<Light>().light_type = LightTypes::Directional;
			new_ntt.transform->rotation = glm::vec3(25.0f, 205.0f, 0.0f);
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Point Light", button_size)) {
			EntityObject new_ntt = scene->CreateEntity("Point Light");
			new_ntt.transform->AddComponent<Light>();
			new_ntt.transform->GetComponent<Light>().light_type = LightTypes::Point;
			new_ntt.transform->GetComponent<Light>().radius_o = 10.0f;
			new_ntt.transform->GetComponent<Light>().radius_i = 0.5f;
			new_ntt.transform->GetComponent<Light>().intensity = 0.600;
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		
		if (ImGui::Button("Create Spot Light", button_size)) {
			EntityObject new_ntt = scene->CreateEntity("Spot Light");
			new_ntt.transform->AddComponent<Light>();
			new_ntt.transform->GetComponent<Light>().light_type = LightTypes::Spotlight;
			new_ntt.transform->GetComponent<Light>().radius_o = 10.0f;
			new_ntt.transform->GetComponent<Light>().radius_i = 0.5f;
			new_ntt.transform->GetComponent<Light>().intensity = 0.600;
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}


		ImGui::Spacing(); ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();

		if (ImGui::Button("Create Camera", button_size)) {
			EntityObject new_ntt = scene->CreateEntity("Camera");
			new_ntt.transform->AddComponent<Camera>(1920, 1080);
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();

		if (ImGui::Button("Create Cube", button_size)) {
			EntityObject new_ntt = scene->CreateEntity("Cube");
			new_ntt.transform->AddComponent<MeshRenderer>(std::make_shared<Model>(Constants::Shapes::Cube()), std::make_shared<Material>(MaterialFlags_Lit | MaterialFlags_Shadow));
			new_ntt.transform->GetComponent<MeshRenderer>().material->color = Constants::Colors::White;
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Sphere", button_size)) {
			EntityObject new_ntt = scene->CreateEntity("Sphere");
			new_ntt.transform->AddComponent<MeshRenderer>(std::make_shared<Model>(Constants::Shapes::UVSphere()), std::make_shared<Material>(MaterialFlags_Lit | MaterialFlags_Shadow));
			new_ntt.transform->GetComponent<MeshRenderer>().material->color = Constants::Colors::White;
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Plane", button_size)) {
			EntityObject new_ntt = scene->CreateEntity("Plane");
			new_ntt.transform->AddComponent<MeshRenderer>(std::make_shared<Model>(Constants::Shapes::Plane()), std::make_shared<Material>(MaterialFlags_Lit | MaterialFlags_Shadow));
			new_ntt.transform->GetComponent<MeshRenderer>().material->color = Constants::Colors::White;
			selected_entity = new_ntt.transform->entity;
			new_ntt.~EntityObject();
			ImGui::CloseCurrentPopup();
		}
		custom_style->ItemSpacing = org_space;

		ImGui::EndPopup();
	}

	//ImGui::SetNextWindowSize(ImVec2(110, 80));
	ImGui::SetNextWindowPos(ImVec2((float)m_x, (float)m_y), ImGuiCond_Once); // simplified
	if (ImGui::BeginPopup("Change Object", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Change Object");
		ImGui::Separator();
		if (ImGui::Button("Duplicate", ImVec2(110, 20))) {
			scene->Scene_ECS.DuplicateEntity(selected_entity);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Delete", ImVec2(110, 20))) {
			scene->Scene_ECS.DestroyEntity(selected_entity);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}