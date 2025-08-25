#include "BoxCollider.h"
#include "Mesh.h"
#include "Entity.h"

void BoxCollider::CalculateCorners() {
	if (init) {
		Mesh* v_mesh = entity->GetComponent<MeshRenderer>().mesh;
		vertices.clear();
		vertices.insert(vertices.begin(), v_mesh->vertices.begin(), v_mesh->vertices.begin() + 8);
	}

	if (transform->rotation != last_rotation || transform->position != last_position || init) {
		Mesh* m_mesh = entity->GetComponent<MeshRenderer>().mesh;

		last_rotation = transform->rotation;
		last_position = transform->position;
		vert_positions.clear();

		for (size_t i = 0; i < 8; i++)
		{
			glm::vec3 tb_local = vertices[i].pos + offset;
			vert_positions.push_back(glm::vec3(m_mesh->modelMatrix * glm::vec4(tb_local, 1.0f)));
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