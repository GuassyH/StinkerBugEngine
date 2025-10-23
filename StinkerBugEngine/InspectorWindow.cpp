#include "InspectorWindow.h"

#include "ComponentsList.h"
#include "CharleyComponent.h"
#include "EntityObject.h"

/// THIS IS THE ENTITY INSPECTOR


char buff[255];
void InspectorWindow::Draw(Scene* scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {
	ImGui::Begin("Entity Inspector");
	
	if (!scene) { ImGui::End(); return; }

	if (scene->Scene_ECS.entities.contains(selected_entity) && is_entity_selected) {


		std::ostringstream ss; ss << scene->Scene_ECS.entity_names.find(selected_entity)->second;
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - ImGui::CalcTextSize(ss.str().c_str()).x * 0.5f);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 180 * 0.5f);
		ImGui::SetNextItemWidth(180);

		if (ImGui::InputTextWithHint(" ", ss.str().c_str(), buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll)) {
			scene->Scene_ECS.entity_names[selected_entity] = buff;
			memset(buff, 0, 255);
		}
		ImGui::SameLine();
		if (ImGui::Button("U")) {	// You can now delete entities but adding new ones after deleting doesnt work
			scene->Scene_ECS.DestroyEntity(selected_entity);
			is_entity_selected = false;
			goto BreakInspector;
		}


		// Foreach type, get all the components and draw in inspector
		// Collapsing component in one entity collapses the same component in ALL entities
		// Probably because the CollapsingHeader name is the same so ID is the same
		scene->Scene_ECS.GetComponent<Transform>(selected_entity).DrawOnInspector();
		ImGui::Separator();
		for (auto& [type, map] : scene->Scene_ECS.components) {
			auto compPtr = map.find(selected_entity);
			if (compPtr != map.end() && compPtr->second && type != typeid(Transform)) {
				Component* c = dynamic_cast<Component*>(compPtr->second.get());
				if (!c->InspectorHasInit) { c->DrawOnInspectorInit(); c->InspectorHasInit = true; }
				c->DrawOnInspector();
				ImGui::Separator();
			}
		}
		if (scene->Scene_ECS.HasComponent<Collider>(selected_entity))
		{
			scene->Scene_ECS.GetComponent<Collider>(selected_entity).DrawOnInspector(); ImGui::Separator();
		}



		// Center button
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 125);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);

		if (ImGui::Button("Add Component", ImVec2(250, 25))) {
			ImGui::OpenPopup("Add Component");
		}



		// Setup popup
		ImGui::SetNextWindowPos(ImVec2(display.windowWidth - 300, display.windowHeight - 300)); // simplified
		// ImGui::SetNextWindowSize(ImVec2(235, 300));
		if (ImGui::BeginPopup("Add Component", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - ImGui::CalcTextSize("Add Component").x * 0.5f);
			ImGui::Text("Add Component");
			ImGui::Separator();
			ImGui::Text("Select a component to add:");
			ImGui::Separator();

			ImGuiStyle* custom_style = &ImGui::GetStyle();
			ImVec2 org_space = custom_style->ItemSpacing;
			custom_style->ItemSpacing = ImVec2(10, 1);

			// I mean it works but its not efficient. Should be a loop for each component type add component like with the DrawOnInspector
			if (!scene->Scene_ECS.HasComponent<MeshRenderer>(selected_entity)) {
				if (ImGui::Button("Mesh Renderer", ImVec2(235, 20))) {
					EntityObject new_ntt; new_ntt.transform = scene->Scene_ECS.GetComponentPtr<Transform>(selected_entity);
					new_ntt.transform->AddComponent<MeshRenderer>(std::make_shared<Model>(Constants::Shapes::Cube()), std::make_shared<Material>(MaterialFlags_Lit | MaterialFlags_Shadow));
					new_ntt.transform->GetComponent<MeshRenderer>().material->color = Constants::Colors::White;
					ImGui::CloseCurrentPopup();
					new_ntt.~EntityObject();
				}
			}
			
			if (!scene->Scene_ECS.HasComponent<Camera>(selected_entity)) {
				if (ImGui::Button("Camera", ImVec2(235, 20))) {
					scene->Scene_ECS.AddComponent<Camera>(selected_entity, 1920, 1080);
					ImGui::CloseCurrentPopup();
				}
			}
			
			if (!scene->Scene_ECS.HasComponent<Light>(selected_entity)) {
				if (ImGui::Button("Light", ImVec2(235, 20))) {
					scene->Scene_ECS.AddComponent<Light>(selected_entity);
					ImGui::CloseCurrentPopup();
				}
			}

			if (!scene->Scene_ECS.HasComponent<RigidBody>(selected_entity)) {
				if (ImGui::Button("Rigidbody", ImVec2(235, 20))) {
					scene->Scene_ECS.AddComponent<RigidBody>(selected_entity);
					ImGui::CloseCurrentPopup();
				}
			}

			if (!scene->Scene_ECS.HasComponent<BoxCollider>(selected_entity) && !scene->Scene_ECS.HasComponent<Collider>(selected_entity)) {
				if (ImGui::Button("BoxCollider", ImVec2(235, 20))) {
					scene->Scene_ECS.AddComponent<BoxCollider>(selected_entity);
					ImGui::CloseCurrentPopup();
				}
			}

			if (!scene->Scene_ECS.HasComponent<SphereCollider>(selected_entity) && !scene->Scene_ECS.HasComponent<Collider>(selected_entity)) {
				if (ImGui::Button("SphereCollider", ImVec2(235, 20))) {
					scene->Scene_ECS.AddComponent<SphereCollider>(selected_entity);
					ImGui::CloseCurrentPopup();
				}
			}

			if (!scene->Scene_ECS.HasComponent<OskarComponent>(selected_entity)) {
				if (ImGui::Button("Oskar Component", ImVec2(235, 20))) {
					scene->Scene_ECS.AddComponent<OskarComponent>(selected_entity);
					ImGui::CloseCurrentPopup();
				}
			}
			if (!scene->Scene_ECS.HasComponent<CharleyComponent>(selected_entity)) {
				if (ImGui::Button("Charley Component", ImVec2(235, 20))) {
					scene->Scene_ECS.AddComponent<CharleyComponent>(selected_entity);
					ImGui::CloseCurrentPopup();
				}
			}

			custom_style->ItemSpacing = org_space;

			ImGui::EndPopup();
		}


	}

BreakInspector:
	ImGui::End();
}
