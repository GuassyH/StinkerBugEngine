#include "Mesh.h"

#include "SceneManager.h"

SceneManager& sceneManager = SceneManager::getInstance();
// Constructor for vector (dynamic size)
Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds, Material& material)
	: vertices(verts), indices(inds), material(&material), ID(sceneManager.GetActiveScene()->meshes.size()) {
	sceneManager.RegisterMesh(this);
	RecalculateMesh();
}

Mesh::Mesh(const Constants::Shapes::Shape& shape, Material& material)
	: vertices(shape.getVertices()), indices(shape.getIndices()), material(&material), ID(sceneManager.GetActiveScene()->meshes.size()) {
	sceneManager.RegisterMesh(this);
	RecalculateMesh();
}

// Constructor for array (fixed size)
template <size_t NVerts, size_t NInds>
Mesh::Mesh(const std::array<Vertex, NVerts>& verts, const std::array<uint32_t, NInds>& inds, Material& material)
	: vertices(verts.begin(), verts.end()), indices(inds.begin(), inds.end()), material(&material), ID(sceneManager.GetActiveScene()->meshes.size()) {
	sceneManager.RegisterMesh(this);
	RecalculateMesh();
}

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

void Mesh::Draw(Camera& camera) {
	Shader& r_shader = material->shader;
	r_shader.Use();

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


	glUniformMatrix4fv(glGetUniformLocation(r_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(r_shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rotMatrix));
	glUniformMatrix4fv(glGetUniformLocation(r_shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera.camMatrix));
	glUniform3f(glGetUniformLocation(r_shader.ID, "camPos"), camera.transform.position.x, camera.transform.position.y, camera.transform.position.z);
	glUniform4f(glGetUniformLocation(r_shader.ID, "color"), material->Color.r, material->Color.g, material->Color.b, material->Color.a);

	if (material->Lit && SceneManager::getInstance().GetActiveScene()) {
		glm::vec3 l_dir = SceneManager::getInstance().GetActiveScene()->light_direction;
		glm::vec3 l_col = SceneManager::getInstance().GetActiveScene()->light_color;
		glUniform3f(glGetUniformLocation(r_shader.ID, "lightDir"), l_dir.x, l_dir.y, l_dir.z);
		glUniform4f(glGetUniformLocation(r_shader.ID, "lightColor"), l_col.r, l_col.g, l_col.b, 1.0f);
		glUniform1f(glGetUniformLocation(r_shader.ID, "ambient"), SceneManager::getInstance().GetActiveScene()->ambient);

	}

	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}



Mesh::~Mesh() { 
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	// Yay
}