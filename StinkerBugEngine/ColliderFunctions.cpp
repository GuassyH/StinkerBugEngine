#include "ColliderFunctions.h"

#include "Constants.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

#include "Entity.h"
#include "Vertex.h"
#include "Mesh.h"


void DebugVec3(std::string text, glm::vec3 vector) {
    std::cout << text << ": " << vector.x << "x " << vector.y << "y " << vector.z << "z\n";
}

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
		collision_info.normal = -glm::normalize(delta);
		collision_info.did_collide = true;
		collision_info.penetration = glm::length(delta) - radius_sum;
	}

	return collision_info;
}

// SAT but with sphere? Wont work
CollisionInfo ColliderFunctions::SphereVsBox(SphereCollider& sphere, BoxCollider& box) {
	CollisionInfo collision_info;
	collision_info.did_collide = false;
/*
    // --- Transform vertices into world space ---
    std::vector<glm::vec3> box_vert_positions;
    glm::vec3 sphere_closest_pos;
    glm::vec3 sphere_farthest_pos;

    glm::vec3 dir = glm::normalize(box.transform->position - sphere.transform->position);
    sphere_closest_pos = sphere.transform->position + (dir * sphere.radius);
    sphere_farthest_pos = sphere.transform->position - (dir * sphere.radius);

    // the meshes HAVE to be different or at least the model matrices need to be
    // Otherwise the collision isnt accurate AT ALL
    Mesh* box_mesh = box.entity->GetComponent<MeshRenderer>().mesh;
    Mesh* sphere_mesh = sphere.entity->GetComponent<MeshRenderer>().mesh;

    for (size_t i = 0; i < 8; i++) {
        // Center vertices around origin
        glm::vec3 box_local = box.vert_positions[i].pos + box.offset;

        box_vert_positions.push_back(glm::vec3(box_mesh->modelMatrix * glm::vec4(box_local, 1.0f)));
    }
*/


	return collision_info;
}

// SEPERATING AXIS THEOREM
CollisionInfo ColliderFunctions::BoxVsBox(BoxCollider& this_box, BoxCollider& other_box) {
    CollisionInfo collision_info;
    collision_info.did_collide = true;

    // --- Transform vertices into world space ---
    std::vector<glm::vec3> tb_vert_positions;
    std::vector<glm::vec3> ob_vert_positions;

    // the meshes HAVE to be different or at least the model matrices need to be
    // Otherwise the collision isnt accurate AT ALL
    Mesh* tb_mesh = this_box.entity->GetComponent<MeshRenderer>().mesh;
    Mesh* ob_mesh = other_box.entity->GetComponent<MeshRenderer>().mesh;

    for (size_t i = 0; i < 8; i++) {
        // Center vertices around origin
        glm::vec3 tb_local = this_box.vert_positions[i].pos + this_box.offset;
        glm::vec3 ob_local = other_box.vert_positions[i].pos + other_box.offset;

        tb_vert_positions.push_back(glm::vec3(tb_mesh->modelMatrix * glm::vec4(tb_local, 1.0f)));
        ob_vert_positions.push_back(glm::vec3(ob_mesh->modelMatrix * glm::vec4(ob_local, 1.0f)));
    }

    // --- Generate axes ---
    std::vector<glm::vec3> all_axis;

    glm::vec3 tb_right      = tb_mesh->modelMatrix[0]; // X
    glm::vec3 tb_up         = tb_mesh->modelMatrix[1]; // Y
    glm::vec3 tb_forward    = tb_mesh->modelMatrix[2]; // Z

    glm::vec3 ob_right      = ob_mesh->modelMatrix[0]; // X
    glm::vec3 ob_up         = ob_mesh->modelMatrix[1]; // Y
    glm::vec3 ob_forward    = ob_mesh->modelMatrix[2]; // Z

    // Add face axes
    all_axis.push_back(glm::normalize(tb_right));
    all_axis.push_back(glm::normalize(tb_up));
    all_axis.push_back(glm::normalize(tb_forward));

    all_axis.push_back(glm::normalize(ob_right));
    all_axis.push_back(glm::normalize(ob_up));
    all_axis.push_back(glm::normalize(ob_forward));

    // Add edge cross-product axes
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 3; j < 6; j++) {
            glm::vec3 crossAxis = glm::cross(all_axis[i], all_axis[j]);
            if (glm::length(crossAxis) > 0.001f) {
                all_axis.push_back(glm::normalize(crossAxis));
            }
        }
    }

    // --- SAT Test ---
    float min_penetration = FLT_MAX;
    glm::vec3 sat_normal = glm::vec3(1.0f);

    glm::vec3 this_box_pos = this_box.transform->position;
    glm::vec3 other_box_pos = other_box.transform->position;


    for (auto& axis : all_axis) {
        if (glm::length(axis) < 0.001f) { continue; }

        float minA = 20070409.0f, maxA = -20070409.0f;
        float minB = 20070409.0f, maxB = -20070409.0f;


        for (size_t i = 0; i < 8; i++) {
            // Project A vertex
            float projA = glm::dot(tb_vert_positions[i], axis);
            minA = glm::min(minA, projA);
            maxA = glm::max(maxA, projA);

            // Project B vertex
            float projB = glm::dot(ob_vert_positions[i], axis);
            minB = glm::min(minB, projB);
            maxB = glm::max(maxB, projB);
        }

        // Check for separation
        bool is_seperated = (minA > maxB + 1e-6f) || (maxA < minB - 1e-6f);
        if(is_seperated){ 
            // std::cout << "no col\n";
            collision_info.did_collide = false;
            return collision_info;
        }

        // Compute penetration
        float overlap1 = maxA - minB;
        float overlap2 = maxB - minA;
        float penetration = glm::min(overlap1, overlap2);

        if (penetration < min_penetration) {
            min_penetration = penetration;
            sat_normal = axis;
        }
    }

    // --- Collision info ---
    collision_info.normal = glm::dot(sat_normal, (other_box_pos - other_box.offset) - (this_box_pos - this_box.offset)) < 0 ? sat_normal : -sat_normal;
    collision_info.penetration = min_penetration;
    
    return collision_info;
}




bool ColliderFunctions::AABB(Collider& first_collider, Collider& second_collider) {

	glm::vec3 this_pos = first_collider.transform->position;
	glm::vec3 other_pos = second_collider.transform->position;

    // the multiplication is temporary
	bool collides_on_x = (this_pos.x + (first_collider.size.x * 2) + first_collider.offset.x > other_pos.x - (second_collider.size.x * 2) + second_collider.offset.x) &&
		(this_pos.x - (first_collider.size.x * 2) + first_collider.offset.x < other_pos.x + (second_collider.size.x * 2) + second_collider.offset.x);

	bool collides_on_y = (this_pos.y + (first_collider.size.y * 2) + first_collider.offset.y > other_pos.y - (second_collider.size.y * 2) + second_collider.offset.y) &&
		(this_pos.y - (first_collider.size.y * 2) + first_collider.offset.y < other_pos.y + (second_collider.size.y * 2) + second_collider.offset.y);

	bool collides_on_z = (this_pos.z + (first_collider.size.z * 2) + first_collider.offset.z > other_pos.z - (second_collider.size.z * 2) + second_collider.offset.z) &&
		(this_pos.z - (first_collider.size.z * 2) + first_collider.offset.z < other_pos.z + (second_collider.size.z * 2) + second_collider.offset.z);


	return collides_on_x && collides_on_y && collides_on_z;
}


/*
std::vector<glm::vec3> contactNormals;
for (size_t j = 0; j < 8; j++) {
	// Transform Box A corner into Box B local space
	glm::vec3 localPos = glm::inverse(ob_mesh->modelMatrix) * glm::vec4(tb_vert_positions[j], 1.0f);

	// Clamp to Box B half-sizes
	glm::vec3 clampedLocal;
	clampedLocal.x = glm::clamp(localPos.x, -other_box.size.x, other_box.size.x);
	clampedLocal.y = glm::clamp(localPos.y, -other_box.size.y, other_box.size.y);
	clampedLocal.z = glm::clamp(localPos.z, -other_box.size.z, other_box.size.z);

	// Transform back to world space
	glm::vec3 closestPoint = ob_mesh->modelMatrix * glm::vec4(clampedLocal, 1.0f);

	// Compute normal from Box B to Box A corner
	glm::vec3 contactNormal = glm::normalize(tb_vert_positions[j] - closestPoint);
	contactNormals.push_back(contactNormal);
}

glm::vec3 averaged_normal(0.0f);
for (auto& n : contactNormals) {
	averaged_normal += n;
}

averaged_normal = glm::normalize(averaged_normal);
*/