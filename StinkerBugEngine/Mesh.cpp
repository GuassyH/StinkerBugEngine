#include "Mesh.h"

#include "SceneManager.h"
#include "Scene.h"
#include "EntityHelper.h"
#include "Transform.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"

SceneManager& sceneManager = SceneManager::getInstance();


Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds)
	: vertices(verts), indices(inds) {
	RecalculateMesh();
}	// Constructor for vector (dynamic size)

Mesh::Mesh(const Constants::Shapes::Shape& shape)
	: vertices(shape.getVertices()), indices(shape.getIndices()) {
	RecalculateMesh();
}									// Constructor for custom constant Shape

template <size_t NVerts, size_t NInds>														// Constructor for array (fixed size)
Mesh::Mesh(const std::array<Vertex, NVerts>& verts, const std::array<uint32_t, NInds>& inds)
	: vertices(verts.begin(), verts.end()), indices(inds.begin(), inds.end()) {
	RecalculateMesh();
}


void Mesh::RecalculateMesh() {
	// The VAO is being overwritten or something like that
	//	   VBO
	//	   EBO

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

void Mesh::render(Material* m_material, Transform* m_transform, Transform* c_transform, Camera* cam, Light* light, bool shadowPass) {
	
	if (!m_transform) { return; }
	m_transform->UpdateMatrix();

	if (shadowPass) {
		VAO1.Bind();
		EBO1.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		return;
	}

	if (!m_material || !light || !cam || !m_transform || !c_transform) { return; }

	m_material->shader.Use();

	glUniform1i(glGetUniformLocation(m_material->shader.ID, "ShadowMap"), 0);
	glUniformMatrix4fv(glGetUniformLocation(m_material->shader.ID, "light_VP"), 1, GL_FALSE, glm::value_ptr(light->light_VP));
	glUniformMatrix4fv(glGetUniformLocation(m_material->shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_transform->GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(m_material->shader.ID, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(m_transform->GetRotationMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(m_material->shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cam->CameraMatrix));
	glUniform3f(glGetUniformLocation(m_material->shader.ID, "camPos"), c_transform->position.x, c_transform->position.y, c_transform->position.z);
	glUniform4f(glGetUniformLocation(m_material->shader.ID, "color"), m_material->Color.r, m_material->Color.g, m_material->Color.b, m_material->Color.a);


	if (m_material->Lit) {
		glm::vec3 l_dir = light->vec_direction;
		glm::vec3 l_col = light->color;
		glUniform3f(glGetUniformLocation(m_material->shader.ID, "lightDir"), l_dir.x, l_dir.y, l_dir.z);
		glUniform4f(glGetUniformLocation(m_material->shader.ID, "lightColor"), l_col.r, l_col.g, l_col.b, 1.0f);
		glUniform1f(glGetUniformLocation(m_material->shader.ID, "ambient"), SceneManager::getInstance().GetActiveScene().ambient);
	}

	VAO1.Bind();
	EBO1.Bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh() { 
	/// IT WAS THIS

	//VAO1.Delete();
	//VBO1.Delete();
	//EBO1.Delete();
}