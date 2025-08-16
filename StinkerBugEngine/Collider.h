#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include <iostream>
#include <memory>
#include "glm/glm.hpp"


class Collider {
public:
  
    Collider() = default;
    virtual ~Collider() = default; // Always virtual destructor for base classes
	virtual glm::vec3 CheckCollisions(const Collider& other) const = 0;

};


#endif