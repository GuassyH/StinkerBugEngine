#ifndef MESH_RENDERER_COMPONENT_H
#define MESH_RENDERER_COMPONENT_H

#include <iostream>
#include <vector>

#include "Constants.h"
#include "Component.h"

#include "Model.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Material.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class MeshRenderer : public Component{
public:
	Model* model = nullptr;
	Material* material = nullptr;

	MeshRenderer(Model& m, Material& mat) : model(&m), material(&mat) {}
	MeshRenderer(Model* m, Material* mat) : model(m), material(mat) {}
	MeshRenderer() = default; // still allow default construction

	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("Mesh Renderer")) {
			
			if (model) {
				ImGui::Text("Model: %s", model->name);
			}
			else {
				ImGui::Text("Model: None");
			}
			if (material) {
				ImGui::ColorEdit4("Color", &material->Color.r);
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
				// ImGui::Checkbox("Lit", &material->Lit);
				// ImGui::Checkbox("Depth", &material->Depth);
			}
			else {
				ImGui::Text("Material: None");
			}
			
		}
	}
};



#endif