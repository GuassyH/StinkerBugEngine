#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "MeshRenderer.h"
#include "Transform.h"
#include "GizmoComponent.h"

#include "glm/glm.hpp"
#include <typeindex>
#include <typeinfo>


struct ObjectCall {
	std::shared_ptr<MeshRenderer> renderer;
    std::shared_ptr<Transform> transform;
};

class Renderer {
private:
    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
public:
	static Renderer& getInstance() { static Renderer instance; return instance; }

	std::vector<ObjectCall> opaque_meshes;
    std::vector<ObjectCall> transparent_meshes;

	void clearMeshes() {
		opaque_meshes.clear();
		transparent_meshes.clear();
	}

    void rebuildMeshLists(std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<ComponentCore>>>& components) {

        clearMeshes();

        auto& meshMap = components[std::type_index(typeid(MeshRenderer))];
        auto& transformMap = components[std::type_index(typeid(Transform))];

        for (auto& [id, compPtr] : meshMap) {
            if (!compPtr) continue;
            if (components[std::type_index(typeid(GizmoComponent))].find(id) != components[std::type_index(typeid(GizmoComponent))].end()) { continue; }

            auto rendererPtr = std::static_pointer_cast<MeshRenderer>(compPtr);
            if (!rendererPtr->model || !rendererPtr->material) continue;

            auto itT = transformMap.find(id);
            if (itT == transformMap.end() || !itT->second) continue;
            auto transformPtr = std::static_pointer_cast<Transform>(itT->second);

            // Could perform frustum culling here


            ObjectCall new_call{ std::shared_ptr<MeshRenderer>(rendererPtr), std::shared_ptr<Transform>(transformPtr) };

            if (rendererPtr->material->HasFlag(MaterialFlags_Transparent)) {
                transparent_meshes.push_back(new_call);
            }
            else {
                opaque_meshes.push_back(new_call);
            }
        }
    }

	void sortTransparentMeshes(glm::vec3& origo) {
		std::sort(transparent_meshes.begin(), transparent_meshes.end(), [&origo](const ObjectCall& a, const ObjectCall& b) {
			float da = glm::length2(origo - a.transform->position);
			float db = glm::length2(origo - b.transform->position);
			return da > db;
		});
	}
};




#endif