#ifndef COMPONENTS_LIST_H
#define COMPONENTS_LIST_H

#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "Light.h"
#include "Mesh.h"
#include "OskarComponent.h"

#include "Collider.h"
#ifdef COLLIDER_COMPONENT_H
#include "SphereCollider.h"
#include "BoxCollider.h"
#endif

#include <functional>
#include "ComponentTypeID.h"

struct ComponentInfo {
    const char* name;
    std::function<std::shared_ptr<Component>()> create;
};


#endif