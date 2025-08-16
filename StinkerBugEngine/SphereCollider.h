#ifndef SPHERE_COLLIDER_COMPONENT_H
#define SPHERE_COLLIDER_COMPONENT_H

#include "Collider.h"

class SphereCollider : public Collider {
public:
	float radius;
	glm::vec3 position;
	glm::vec3 offset = glm::vec3(0.5);

    
	glm::vec3 CheckCollisions(const Collider& other) const override {
		const SphereCollider* sphere = dynamic_cast<const SphereCollider*>(&other);

        if (sphere) {
            // Now you can safely use 'sphere'
            float distance = glm::distance(position, sphere->position);
            if (distance <= radius + sphere->radius) {
                std::cout << "Collision detected!\n";
                return glm::normalize(sphere->position - position);
            }
            else {
                std::cout << "Other collider is not a sphere.\n";
                return glm::vec3(0.0);
            }
        }
        else {
            std::cout << "Other collider is not a sphere.\n";
            return glm::vec3(0.0);
        }
	}
};


#endif