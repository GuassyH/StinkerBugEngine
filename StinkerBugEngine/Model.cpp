#include "Model.h"

Model::Model(Mesh mesh) {
	meshes.push_back(mesh);
}

void Model::init() {}

void Model::render(Shader shader, Material* m_material, Transform* m_transform, Transform* c_transform, Camera* cam, Light* light, bool shadowPass) {
	for (Mesh mesh : meshes) {
		mesh.render(shader, m_material, m_transform, c_transform, cam, light, shadowPass);
	}
}
