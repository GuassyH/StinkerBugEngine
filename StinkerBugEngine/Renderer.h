#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "glm/glm.hpp"
#include <typeindex>
#include <typeinfo>
#include "Entity.h"

class Component;
class GizmoComponent;
class Transform;
class MeshRenderer;

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

	void rebuildMeshLists(std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<Component>>>& components);
	void sortTransparentMeshes(glm::vec3& origo);
};




#endif