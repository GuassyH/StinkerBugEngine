#ifndef COMPONENTS_LIST_H
#define COMPONENTS_LIST_H

#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "Light.h"
#include "Mesh.h"
#include "TestComponent.h"

#include "Collider.h"
#ifdef COLLIDER_COMPONENT_H
#include "SphereCollider.h"
#include "BoxCollider.h"
#endif

// #include <unordered_map>
// #include <typeindex>

// std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<Component>>> components_list_components;


#endif