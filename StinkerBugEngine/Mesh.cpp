#include "Mesh.h"



void Mesh::RecalculateMesh() {
    VAO1 = VAO();
    VBO1 = VBO();
    EBO1 = EBO();

	VBO1.BindBufferData(sizeof(Vertex) * vertices.size(), vertices.data());

	VAO1.LinkAttrib(0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, pos));
	VAO1.LinkAttrib(1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
	VAO1.LinkAttrib(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoords));

	EBO1.BindBufferData(indices.size() * sizeof(GLuint), indices.data());

	VBO1.Unbind();
	VAO1.Unbind();
	EBO1.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera) {
	shader.Use();

	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);

	// Construct rotation quat (Y > X > Z)
	glm::quat rotY = glm::angleAxis(glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat rotX = glm::angleAxis(glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat rotZ = glm::angleAxis(glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat rotationQuat = rotY * rotX * rotZ; // Apply Z rotation first, then X, then Y

	glm::mat4 rotMatrix = glm::mat4_cast(rotationQuat);
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.position);

	// Combine to get model matrix: translate * rotate * scale
	glm::mat4 model = translationMatrix * rotMatrix * scaleMatrix;


	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera.camMatrix));



	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}



Mesh::~Mesh() { 
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	// Yay
}