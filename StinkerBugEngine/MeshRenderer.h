#ifndef MESH_RENDERER_COMPONENT_H
#define MESH_RENDERER_COMPONENT_H

#include <iostream>
#include <vector>

#include "Constants.h"
#include "Component.h"

#include "Material.h"
#include "Vertex.h"
#include "Mesh.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

struct MeshRenderer : public Component{
public:
	Mesh* mesh = nullptr;
	Material* material = nullptr;

	MeshRenderer(Mesh& m, Material& mat) : mesh(&m), material(&mat) {}
	MeshRenderer() = default; // still allow default construction

	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("Mesh Renderer")) {
			if (mesh) {
				ImGui::Text("Mesh: %s", mesh->name);
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
		}
	}
};



#endif