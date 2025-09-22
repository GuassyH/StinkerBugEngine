#include "InspectorWindow.h"

#include "ComponentsList.h"
#include "EntityHelper.h"

/// THIS IS THE ENTITY INSPECTOR
char buff[255];
char* new_name;
void InspectorWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {

	ImGui::SetNextWindowPos(ImVec2(display.windowWidth - 350, 40));
	ImGui::SetNextWindowSize(ImVec2(350, display.windowHeight - 40));
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
		for (auto& [type, map] : scene.Scene_ECS.components) {
			auto compPtr = map.find(selected_entity);
			if (compPtr != map.end() && compPtr->second && type != typeid(Transform)) {
				Component* c = dynamic_cast<Component*>(compPtr->second.get());
				c->DrawOnInspector();
				ImGui::Separator();
			}
		}
		if (scene.Scene_ECS.HasComponent<Collider>(selected_entity))
		{
			scene.Scene_ECS.GetComponent<Collider>(selected_entity).DrawOnInspector(); ImGui::Separator();
		}



		// Center button
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 125);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);

		if (ImGui::Button("Add Component", ImVec2(250, 25))) {
			ImGui::OpenPopup("Add Component");
		}

		// Setup popup


		ImGui::SetNextWindowPos(ImVec2(display.windowWidth - 300, display.windowHeight - 360)); // simplified
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
					EntityHelper new_ntt(selected_entity, &scene.Scene_ECS);
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

			if (!scene.Scene_ECS.HasComponent<Light>(selected_entity)) {
				if (ImGui::Button("Light", ImVec2(235, 20))) {
					scene.Scene_ECS.AddComponent<Light>(selected_entity);
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

			if (!scene.Scene_ECS.HasComponent<OskarComponent>(selected_entity)) {
				if (ImGui::Button("Oskar Component", ImVec2(235, 20))) {
					scene.Scene_ECS.AddComponent<OskarComponent>(selected_entity);
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}


	}

BreakInspector:
	ImGui::End();
}
