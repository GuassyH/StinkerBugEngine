#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <iostream>
#include <memory>

#include "unordered_map"
#include "glm/glm.hpp"
#include "EntityID.h"

#include "Mesh.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Collider.h"

class Scene {
public:
	Scene() = default;

	EntityID nextEntity = 0;

	std::unordered_map<EntityID, Transform> transforms = {};
	std::unordered_map<EntityID, MeshRenderer> meshRenderers = {};
	std::unordered_map<EntityID, std::unique_ptr<Collider>> colliders = {};

	glm::vec3 light_direction = glm::normalize(glm::vec3(-1, -1.3, -0.84));
	glm::vec3 light_color = glm::vec3(1.0);
	float ambient = 0.2f;

	EntityID CreateEntity() {
		return nextEntity++;
	}

	void Draw(Camera& camera) {
		for (auto& [id, renderer] : meshRenderers) {
			auto it = transforms.find(id);
			if (it != transforms.end() && renderer.mesh) {
				Transform& t = it->second;

				renderer.mesh->Draw(camera, renderer.material, t);
			}
		}
	}
};



#endif  