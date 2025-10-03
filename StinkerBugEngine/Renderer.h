#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include <iostream>
#include <vector>

#include "MeshRenderer.h"
#include "Transform.h"
#include "glm/glm.hpp"
#include "Scene.h"

struct ObjectCall {
	MeshRenderer* renderer;
	Transform* transform;
};

class Renderer {
private:
	Renderer() = default;
public:
	static Renderer& getInstance() { static Renderer instance; return instance; }

	std::vector<ObjectCall> opaque_meshes;
	std::vector<ObjectCall> transparent_meshes;

	void clearMeshes() {
		opaque_meshes.clear();
		transparent_meshes.clear();
	}

	void calculateOpaqueMeshes(Scene& scene) {
		for (auto& [id, components_renderer] : scene.Scene_ECS.GetComponentMap<MeshRenderer>()) {
			MeshRenderer& renderer = *std::static_pointer_cast<MeshRenderer>(components_renderer);
			if (!renderer.model || !renderer.material || renderer.material->HasFlag(MaterialFlags_Transparent)) { continue; }	// If there isnt a model and material then skip

			Transform& r_transform = scene.Scene_ECS.GetComponent<Transform>(id);
			ObjectCall new_call{ &renderer, &r_transform };
			opaque_meshes.push_back(new_call);
		}
	}

	void calculateTransparentMeshes(Scene& scene) {
		for (auto& [id, components_renderer] : scene.Scene_ECS.GetComponentMap<MeshRenderer>()) {
			MeshRenderer& renderer = *std::static_pointer_cast<MeshRenderer>(components_renderer);
			if (!renderer.model || !renderer.material || !renderer.material->HasFlag(MaterialFlags_Transparent)) { continue; }	// If there isnt a model and material then skip

			Transform& r_transform = scene.Scene_ECS.GetComponent<Transform>(id);
			ObjectCall new_call{ &renderer, &r_transform };
			opaque_meshes.push_back(new_call);
		}
	}

	void sortTransparentMeshes(glm::vec3& origo) {
		std::sort(transparent_meshes.begin(), transparent_meshes.end(), [origo](const ObjectCall& a, const ObjectCall& b) {
			float da = glm::length2(origo - a.transform->position);
			float db = glm::length2(origo - b.transform->position);
			return da > db;
		});
	}
};




#endif