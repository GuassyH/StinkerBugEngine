#include "BoxCollider.h"
#include "Mesh.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Scene.h"


void BoxCollider::CalculateCorners() {
	Mesh* mesh = entity->GetComponent<MeshRenderer>().mesh;

	vert_positions.clear();
	vert_positions.insert(vert_positions.begin(), mesh->vertices.begin(), mesh->vertices.begin() + 8);

	offset = glm::vec3(0.0f);
}

CollisionInfo BoxCollider::CollideWithBox(BoxCollider& other_box_collider) {
	CalculateCorners();
	other_box_collider.CalculateCorners();
	return ColliderFunctions::BoxVsBox(*this, other_box_collider);
}

CollisionInfo BoxCollider::CollideWithSphere(SphereCollider& other_sphere_collider) {
	return ColliderFunctions::SphereVsBox(other_sphere_collider, *this);
}