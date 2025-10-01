#ifndef PHYSICS_FUNCTIONS_H
#define PHYSICS_FUNCTIONS_H


#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include "Entity.h"

#include "ECSystem.h"
#include "ComponentsList.h"

namespace Physics {

	struct RaycastHit {
		bool hit = false;
		float distance = 0.0f;
		glm::vec3 direction = glm::vec3(0.0);
		glm::vec3 point = glm::vec3(0.0);
		Entity entity = 0;

		// Temporary maybe
		bool isGizmo = false;
	};

	struct Ray {
		glm::vec3 direction;
		glm::vec3 position;
	};


	static inline bool CheckAgainstRay(RaycastHit& rayHit, glm::vec3 rayDir, glm::vec3 position, Mesh& mesh, Transform& m_transform, Camera* camera, glm::vec2 MouseNDC) {
		// Check if the ray intersects, else return


		return false;
	}

	static inline RaycastHit Raycast(glm::vec3 rayDir, glm::vec3 position, float distance, Camera* camera, glm::vec2 MouseNDC, ECSystem& ecs_to_compare) {
		RaycastHit rayHit;
		rayHit.direction = rayDir;
		// How i understant it, go through each object. Check if the ray hits it, if so store it
		// Then repeat for all objects, if its closer overwrite previous data
		for (auto& [id, components_renderer] : ecs_to_compare.GetComponentMap<MeshRenderer>()) {
			MeshRenderer& renderer = *std::static_pointer_cast<MeshRenderer>(components_renderer);
			if (!renderer.model || !renderer.raycastable) { continue; }	// If there isnt a model skip
			Transform& m_transform = ecs_to_compare.GetComponent<Transform>(id);
			Model& model = *renderer.model;

			for (auto& mesh : model.getMeshes()) {
				if (CheckAgainstRay(rayHit, rayDir, position, mesh, m_transform, camera, MouseNDC)) {
					// do shi
				}
			}
		}

		return rayHit;
	}

	static inline RaycastHit Raycast(Ray ray, float distance, Camera* camera, glm::vec2 MouseNDC, ECSystem& ecs_to_compare) {
		RaycastHit rayHit;
		rayHit.direction = ray.direction;

		// How i understant it, go through each object. Check if the ray hits it, if so store it
		// Then repeat for all objects, if its closer overwrite previous data
		for (auto& [id, components_renderer] : ecs_to_compare.GetComponentMap<MeshRenderer>()) {
			MeshRenderer& renderer = *std::static_pointer_cast<MeshRenderer>(components_renderer);
			if (!renderer.model || !renderer.raycastable) { continue; }	// If there isnt a model skip
			Transform& m_transform = ecs_to_compare.GetComponent<Transform>(id);
			Model& model = *renderer.model;

			for (auto& mesh : model.getMeshes()) {
				if (CheckAgainstRay(rayHit, ray.direction, ray.position, mesh, m_transform, camera, MouseNDC)) {
					// do shi
				}
			}
		}


		return rayHit;
	}

}



#endif