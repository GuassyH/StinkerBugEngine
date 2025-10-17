#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "glm/glm.hpp"
#include <typeindex>
#include <typeinfo>
#include "Entity.h"
#include "WeakPtrProxy.h"
#include "GLFW/glfw3.h"

class Component;
class GizmoComponent;
class Transform;
class MeshRenderer;

struct ObjectCall {
	WeakPtrProxy<MeshRenderer> renderer;
	WeakPtrProxy<Transform> transform;
};


class Renderer {
private:
    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

	GLuint lightBufferID;
	int lightBuffSize;
	int numLights;
public:
	static Renderer& getInstance() { static Renderer instance; return instance; }

	// MESH CALCS
	bool queue_mesh_rebuild = false;

	std::vector<ObjectCall> opaque_meshes;
    std::vector<ObjectCall> transparent_meshes;

	void clearMeshes() {
		opaque_meshes.clear();
		transparent_meshes.clear();
	}

	void rebuildMeshLists(std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<Component>>>& components);
	void sortTransparentMeshes(glm::vec3& origo);

	// LIGHTING CALCS
	

	bool queue_lights_rebuild = false;

	void recalculateLightsBuffer(std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<Component>>>& components);
	void bindLightsBuffer();
	void deleteLightsBuffer();
	int GetNumLights() { return numLights; }
};




#endif