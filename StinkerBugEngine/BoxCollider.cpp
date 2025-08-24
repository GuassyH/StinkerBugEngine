#include "BoxCollider.h"
#include "Mesh.h"
#include "Entity.h"

glm::vec3 last_rotation;
void BoxCollider::CalculateCorners() {
	if (transform->rotation != last_rotation) {
		Mesh* mesh = entity->GetComponent<MeshRenderer>().mesh;

		vert_positions.clear();
		vert_positions.insert(vert_positions.begin(), mesh->vertices.begin(), mesh->vertices.begin() + 8);

		last_rotation = transform->rotation;
	}
}

CollisionInfo BoxCollider::CollideWithBox(BoxCollider& other_box_collider) {
	CalculateCorners();
	other_box_collider.CalculateCorners();
	return ColliderFunctions::BoxVsBox(*this, other_box_collider);
}

CollisionInfo BoxCollider::CollideWithSphere(SphereCollider& other_sphere_collider) {
	return ColliderFunctions::SphereVsBox(other_sphere_collider, *this);
}