#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include <iostream>
#include <memory>

class Collider {
public:
    Collider() = default;
    virtual ~Collider() = default; // Always virtual destructor for base classes
	virtual void CheckCollisions(const Collider& other) const = 0;

};


#endif