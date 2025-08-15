#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "glm/glm.hpp"

class Mesh;
class Collider;

struct Scene {
	std::vector<Mesh*> meshes = {};
	std::vector<Collider*> colliders = {};

	glm::vec3 light_direction = glm::normalize(glm::vec3(-1, -1.3, -0.84));
	glm::vec3 light_color = glm::vec3(1.0);
	float ambient = 0.2f;

	void AddMesh(Mesh* mesh) {
		meshes.push_back(mesh);
	}

};



#endif  