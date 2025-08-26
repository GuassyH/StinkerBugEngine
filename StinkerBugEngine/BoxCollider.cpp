#include "BoxCollider.h"
#include "Mesh.h"
#include "Entity.h"

void BoxCollider::CalculateCorners() {
	if (init) {
		Mesh* v_mesh = entity->GetComponent<MeshRenderer>().mesh;
		vertices.clear();
		vertices.insert(vertices.begin(), v_mesh->vertices.begin(), v_mesh->vertices.begin() + 8);
	}

	if (transform->rotation != last_rotation || transform->position != last_position || transform->scale != last_scale || init) {

		last_rotation = transform->rotation;
		last_position = transform->position;
		last_scale = transform->scale;
		vert_positions.clear();

		for (size_t i = 0; i < 8; i++)
		{
			glm::vec3 tb_local = vertices[i].pos + offset;
			vert_positions.push_back(glm::vec3(transform->GetModelMatrix() * glm::vec4(tb_local, 1.0f)));
		}
	}

	if (init) { init = false; }
}

CollisionInfo BoxCollider::CollideWithBox(BoxCollider& other_box_collider) {
	CalculateCorners();
	other_box_collider.CalculateCorners();
	return ColliderFunctions::BoxVsBox(*this, other_box_collider);
}

CollisionInfo BoxCollider::CollideWithSphere(SphereCollider& other_sphere_collider) {
	return ColliderFunctions::SphereVsBox(other_sphere_collider, *this);
}