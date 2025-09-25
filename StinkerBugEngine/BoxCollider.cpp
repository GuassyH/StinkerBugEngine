#include "BoxCollider.h"
#include "Mesh.h"
#include "EntityHelper.h"

bool BoxCollider::CalculateCorners() {
	// If the box collider isnt setup correctly (doesnt have a mesh) then return false
	// if (!entityHelper->GetComponent<MeshRenderer>().model) { return false; }

	if (transform->rotation != last_rotation || transform->position != last_position || size != last_size || init) {

		last_rotation = transform->rotation;
		last_position = transform->position;
		last_size = size;
		vert_positions.clear();

		for (size_t i = 0; i < 8; i++)
		{
			glm::vec3 tb_local = (corner_positions[i] + offset - glm::vec3(0.5)) * size;
			vert_positions.push_back(glm::vec3(transform->GetModelMatrix() * glm::vec4(tb_local, 1.0f)));
		}
	}

	if (init) { init = false; }
	return true;
}

CollisionInfo BoxCollider::CollideWithBox(BoxCollider& other_box_collider) {
	bool a = CalculateCorners();
	bool b = other_box_collider.CalculateCorners();

	if (!a || !b) { CollisionInfo info; info.did_collide = false; return info; }

	return ColliderFunctions::BoxVsBox(*this, other_box_collider);
}

CollisionInfo BoxCollider::CollideWithSphere(SphereCollider& other_sphere_collider) {
	return ColliderFunctions::SphereVsBox(other_sphere_collider, *this);
}