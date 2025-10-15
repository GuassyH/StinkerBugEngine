#ifndef MESH_RENDERER_COMPONENT_H
#define MESH_RENDERER_COMPONENT_H

#include <iostream>
#include <vector>
#include <memory>

#include "Constants.h"
#include "Component.h"

#include "Model.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Material.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class MeshRenderer : public Component {
public:
	std::shared_ptr<Model> model = nullptr;
	std::shared_ptr<Material> material = nullptr;

	bool raycastable = true;
	bool shadowCaster = true;

	MeshRenderer(Model& m, Material& mat) : model(std::make_shared<Model>(m)), material(std::make_shared<Material>(mat)) {}
	MeshRenderer(std::shared_ptr<Model> m, std::shared_ptr<Material> mat) : model(m), material(mat) {}
	MeshRenderer(Model* w_m, Material* w_mat) = delete;
	MeshRenderer() = default; // still allow default construction

	bool trnsprncy = false;

	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("Mesh Renderer")) {
			ImGui::Checkbox("Shadow Caster", &shadowCaster);
			ImGui::Checkbox("Raycast Target", &raycastable);

			if (model) {
				ImGui::Text("Model: %s", model->name);
			}
			else {
				ImGui::Text("Model: None");
			}

			if (material) {
				ImGui::ColorEdit4("Color", &material->color.r);
				if (ImGui::Selectable("Lit", material->HasFlag(MaterialFlags_Lit), ImGuiSelectableFlags_None, ImVec2(50, 20))) {
					if(material->HasFlag(MaterialFlags_Lit)) {
						material->RemoveFlag(MaterialFlags_Lit);
					}
					else {
						material->AddFlag(MaterialFlags_Lit);
					}
				}
				if (ImGui::Selectable("Depth", material->HasFlag(MaterialFlags_Depth), ImGuiSelectableFlags_None, ImVec2(50, 20))) {
					if (material->HasFlag(MaterialFlags_Depth)) {
						material->RemoveFlag(MaterialFlags_Depth);
					}
					else {
						material->AddFlag(MaterialFlags_Depth);
					}
				}
				if (ImGui::Selectable("Shadow", material->HasFlag(MaterialFlags_Shadow), ImGuiSelectableFlags_None, ImVec2(50, 20))) {
					if (material->HasFlag(MaterialFlags_Shadow)) {
						material->RemoveFlag(MaterialFlags_Shadow);
					}
					else {
						material->AddFlag(MaterialFlags_Shadow);
					}
				}
				if(ImGui::Button("Recompile Shader")) {
					material->Recompile();
				}
				ImGui::Checkbox("Transparent", &trnsprncy);

				if (trnsprncy) {
					material->AddFlag(MaterialFlags_Transparent);
				}
				else {
					material->RemoveFlag(MaterialFlags_Transparent);
				}
			}
			else {
				ImGui::Text("Material: None");
			}
			
		}
	}
};



#endif