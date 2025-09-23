#ifndef MESH_RENDERER_COMPONENT_H
#define MESH_RENDERER_COMPONENT_H

#include <iostream>
#include <vector>

#include "Constants.h"
#include "Component.h"

#include "Model.h"
#include "Vertex.h"
#include "Mesh.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class MeshRenderer : public Component{
public:
	Model* model = nullptr;
	Material* material = nullptr;

	MeshRenderer(Model& m, Material& mat) : model(&m), material(&mat) {}
	MeshRenderer() = default; // still allow default construction

	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("Mesh Renderer")) {
			/*
			if (model) {
				ImGui::Text("Mesh: %s", model->name);
			}
			else {
				ImGui::Text("Mesh: None");
			}
			if (material) {
				ImGui::ColorEdit4("Color", &material->Color.r);
				ImGui::Checkbox("Lit", &material->Lit);
				ImGui::Checkbox("Depth", &material->Depth);
			}
			else {
				ImGui::Text("Material: None");
			}
			*/

		}
	}
};



#endif