#include "Mesh.h"

#include "SceneManager.h"
#include "Scene.h"

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


Mesh::~Mesh() { 
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	// Yay
}