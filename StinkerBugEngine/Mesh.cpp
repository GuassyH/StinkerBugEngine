#include "Mesh.h"

#include "SceneManager.h"
#include "Scene.h"
#include "EntityHelper.h"

#include "Transform.h"
#include "Camera.h"
#include "Light.h"


Mesh::Mesh(const Constants::Shapes::Shape& shape, std::vector<Texture> textures)
	: vertices(shape.getVertices()), indices(shape.getIndices()), textures(textures) {
	RecalculateMesh();
}	// Constructor for custom constant Shape

Mesh::Mesh(std::vector<Vertex> verts, std::vector<uint32_t> inds, std::vector<Texture> textures) 
	: vertices(verts), indices(inds), textures(textures) {
	RecalculateMesh();
}	// Constructor for vector (dynamic size)


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



void Mesh::render(Material* material, Transform* m_transform, Transform* c_transform, Camera* cam, Light* light, bool shadowPass) {

	if(shadowPass) {
		VAO1.Bind();
		EBO1.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		return;
	}

	Shader& shader = material->shader;

	if (!&shader || !light || !cam || !m_transform || !c_transform) { return; }
	shader.Use();

	// Textures should only be done once per material but for now its here
	unsigned int diffuseIdx = 0;
	unsigned int specularIdx = 0;

	/// SINCE SHADOW MAP IS GL_TEXTURE0, START AT GL_TEXTURE1
	for (unsigned int i = 0; i < textures.size(); i++) {
		// activate texture slot
		glActiveTexture(GL_TEXTURE1 + i); // Activate the texture unit first

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
		shader.SetInt(name.c_str(), i + 1); // Set the sampler to the correct texture unit
		// Bind
		textures[i].Bind();
	}

	shader.SetInt("hasDiffuse", diffuseIdx > 0);
	shader.SetInt("hasSpecular", specularIdx > 0);

	// Reset active texture to shadowmap
	cam->m_shadowMapFBO.BindForReading(GL_TEXTURE0);

	// Set uniforms, ILL CHANGE THIS TO BE MORE EFFICIENT LATER
	shader.SetInt("ShadowMap", 0);
	
	// Set matrices
	shader.SetMat4("light_VP", light->light_VP);
	shader.SetMat4("modelMatrix", m_transform->GetModelMatrix());
	shader.SetMat4("rotationMatrix", m_transform->GetRotationMatrix());
	shader.SetMat4("camMatrix", cam->CameraMatrix);
	
	// Others
	shader.SetVec3("camPos", c_transform->position);
	shader.SetVec4("color", material->Color);

	glm::vec3 l_dir = light->vec_direction;
	glm::vec3 l_col = light->color;

	// set dir light uniforms
	shader.SetVec3("lightDir", l_dir);
	shader.SetVec4("lightColor", glm::vec4(l_col, 1.0f));
	shader.SetFloat("ambient", SceneManager::getInstance().GetActiveScene().ambient);

	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

	// Reset all to textures to the shadow map texture since its needed for all objects
	// This should maybe not be done like this but for now it works
	VAO1.Unbind();
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::cleanup() { 
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	for (auto& tex : textures) {
		tex.Delete();
	}
}