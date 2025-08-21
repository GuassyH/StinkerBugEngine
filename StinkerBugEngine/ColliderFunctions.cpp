#include "ColliderFunctions.h"

#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

CollisionInfo ColliderFunctions::SphereVsSphere(SphereCollider& this_sphere, SphereCollider& other_sphere) {
	CollisionInfo collision_info;
	collision_info.did_collide = false;

	glm::vec3 this_pos = this_sphere.transform->position + this_sphere.offset;
	glm::vec3 other_pos = other_sphere.transform->position + other_sphere.offset;

	glm::vec3 delta = other_pos - this_pos;
	float radius_sum = this_sphere.radius + other_sphere.radius;
	float distance_squared = glm::dot(delta, delta);  // Squared distance

	if (distance_squared <= radius_sum * radius_sum) {
		// Only normalize if needed for collision normal
		collision_info.collision_normal = -glm::normalize(delta);
		collision_info.did_collide = true;
	}

	return collision_info;
}


CollisionInfo ColliderFunctions::SphereVsBox(SphereCollider& sphere, BoxCollider& box) {
	CollisionInfo collision_info;
	collision_info.did_collide = false;

	glm::vec3 sphere_pos = sphere.transform->position + sphere.offset;
	glm::vec3 box_pos = box.transform->position;


	return collision_info;
}


CollisionInfo ColliderFunctions::BoxVsBox(BoxCollider& this_box, BoxCollider& other_box) {
	CollisionInfo collision_info;
	collision_info.did_collide = false;

	glm::vec3 this_box_pos = this_box.transform->position;
	glm::vec3 other_box_pos = other_box.transform->position;

	bool collides_on_x = (this_box_pos.x + this_box.size.x + this_box.offset.x > other_box_pos.x - other_box.size.x + other_box.offset.x) &&
		(this_box_pos.x - this_box.size.x + this_box.offset.x < other_box_pos.x + other_box.size.x + other_box.offset.x);

	bool collides_on_y = (this_box_pos.y + this_box.size.y + this_box.offset.y > other_box_pos.y - other_box.size.y + other_box.offset.y) &&
		(this_box_pos.y - this_box.size.y + this_box.offset.y < other_box_pos.y + other_box.size.y + other_box.offset.y);

	bool collides_on_z = (this_box_pos.z + this_box.size.z + this_box.offset.z > other_box_pos.z - other_box.size.z + other_box.offset.z) &&
		(this_box_pos.z - this_box.size.z + this_box.offset.z < other_box_pos.z + other_box.size.z + other_box.offset.z);

	collision_info.did_collide = collides_on_x && collides_on_y && collides_on_z;


	return collision_info;
}




bool ColliderFunctions::AABB(Collider& first_collider, Collider& second_collider) {

	glm::vec3 this_pos = first_collider.transform->position;
	glm::vec3 other_pos = second_collider.transform->position;

	bool collides_on_x = (this_pos.x + first_collider.size.x + first_collider.offset.x > other_pos.x - second_collider.size.x + second_collider.offset.x) &&
		(this_pos.x - first_collider.size.x + first_collider.offset.x < other_pos.x + second_collider.size.x + second_collider.offset.x);

	bool collides_on_y = (this_pos.y + first_collider.size.y + first_collider.offset.y > other_pos.y - second_collider.size.y + second_collider.offset.y) &&
		(this_pos.y - first_collider.size.y + first_collider.offset.y < other_pos.y + second_collider.size.y + second_collider.offset.y);

	bool collides_on_z = (this_pos.z + first_collider.size.z + first_collider.offset.z > other_pos.z - second_collider.size.z + second_collider.offset.z) &&
		(this_pos.z - first_collider.size.z + first_collider.offset.z < other_pos.z + second_collider.size.z + second_collider.offset.z);


	return collides_on_x && collides_on_y && collides_on_z;
}