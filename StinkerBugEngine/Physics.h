#ifndef PHYSICS_FUNCTIONS_H
#define PHYSICS_FUNCTIONS_H


#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include "Entity.h"


namespace Physics {

	struct RaycastHit {
		bool hit = false;
		float distance = 0.0f;
		glm::vec3 point;
		Entity entity;

		// Temporary maybe
		bool isGizmo = false;
	};

	struct Ray {
		glm::vec3 direction;
		glm::vec3 position;
	};

	static inline RaycastHit Raycast(glm::vec3 vector_dir, glm::vec3 position, float distance) {
		RaycastHit rayHit;


		return rayHit;
	}

	static inline RaycastHit Raycast(Ray ray, float distance) {
		RaycastHit rayHit;


		return rayHit;
	}

}



#endif