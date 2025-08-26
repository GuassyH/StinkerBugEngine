#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include "glm/matrix.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>



class Transform {
public:
	Transform() = default;

	Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl)
		: position(pos), rotation(rot), scale(scl),
		modelMatrix(1.0f), rotationMatrix(1.0f) {
	}

	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 scale = glm::vec3(1.0);

	void UpdateMatrix() {
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

		// Construct rotation quat (Y > X > Z)
		glm::quat rotY = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat rotX = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat rotZ = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::quat rotationQuat = rotY * rotX * rotZ; // Apply Z rotation first, then X, then Y

		rotationMatrix = glm::mat4_cast(rotationQuat);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

		// Combine to get model matrix: translate * rotate * scale
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}

	const glm::mat4& GetModelMatrix() const { return modelMatrix; }
	const glm::mat4& GetRotationMatrix() const { return rotationMatrix; }

private:
	glm::mat4 modelMatrix = glm::mat4(1.0);
	glm::mat4 rotationMatrix = glm::mat4(1.0);
};


#endif