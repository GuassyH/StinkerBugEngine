#ifndef SPHERE_COLLIDER_COMPONENT_H
#define SPHERE_COLLIDER_COMPONENT_H


#include "Collider.h"

class SphereCollider : public Collider {
public:
	void CheckCollisions(const Collider& other) const override {
		std::cout << "this is a sphere collider" << std::endl;
	}
};


#endif