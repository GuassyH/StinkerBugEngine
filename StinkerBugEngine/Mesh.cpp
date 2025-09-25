#include "Mesh.h"

#include "SceneManager.h"
#include "Scene.h"
#include "EntityHelper.h"

#include "Transform.h"
#include "Camera.h"
#include "Light.h"

SceneManager& sceneManager = SceneManager::getInstance();

Mesh::Mesh(const Constants::Shapes::Shape& shape, std::vector<Texture> textures)
	: vertices(shape.getVertices()), indices(shape.getIndices()), textures(textures) {
	RecalculateMesh();
}	// Constructor for custom constant Shape

Mesh::Mesh(std::vector<Vertex> verts, std::vector<uint32_t> inds, std::vector<Texture> textures) 
	: vertices(verts), indices(inds), textures(textures) {
	RecalculateMesh();
}	// Constructor for vector (dynamic size)


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



void Mesh::render(Shader& shader, Transform* m_transform, Transform* c_transform, Camera* cam, Light* light, bool shadowPass) {

	if(shadowPass) {
		VAO1.Bind();
		EBO1.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		return;
	}

	if (!&shader || !light || !cam || !m_transform || !c_transform) { return; }
	shader.Use();

	// Textures
	unsigned int diffuseIdx = 0;
	unsigned int specularIdx = 0;

	for (unsigned int i = 0; i < textures.size(); i++) {
		// activate texture slot
		glActiveTexture(GL_TEXTURE0 + i); // Activate the texture unit first

		// retrieve texture number (the N in diffuse_textureN)
		std::string name;
		switch (textures[i].type) {
		case aiTextureType_DIFFUSE:
			name = "diffuse" + std::to_string(diffuseIdx++);
			break;
		case aiTextureType_SPECULAR:
			name = "specular" + std::to_string(specularIdx++);
			break;
		}
		// Set shader texture value
		glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), i);
		// Bind
		textures[i].Bind();
	}

	// Set uniforms, ILL CHANGE THIS TO BE MORE EFFICIENT LATER
	glUniform1i(glGetUniformLocation(shader.ID, "ShadowMap"), 0);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "light_VP"), 1, GL_FALSE, glm::value_ptr(light->light_VP));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_transform->GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(m_transform->GetRotationMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cam->CameraMatrix));
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), c_transform->position.x, c_transform->position.y, c_transform->position.z);
	// glUniform4f(glGetUniformLocation(shader.ID, "color"), m_material->Color.r, m_material->Color.g, m_material->Color.b, m_material->Color.a);

	glm::vec3 l_dir = light->vec_direction;
	glm::vec3 l_col = light->color;
	glUniform3f(glGetUniformLocation(shader.ID, "lightDir"), l_dir.x, l_dir.y, l_dir.z);
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), l_col.r, l_col.g, l_col.b, 1.0f);
	glUniform1f(glGetUniformLocation(shader.ID, "ambient"), SceneManager::getInstance().GetActiveScene().ambient);

	VAO1.Bind();
	EBO1.Bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	VAO1.Unbind();

	glActiveTexture(0);
}

void Mesh::cleanup() { 
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	for (auto& tex : textures) {
		tex.Delete();
	}
}