#ifndef MESH_CLASS_H
#define MESH_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <array>

#include <glm/glm.hpp>
#include "glm/matrix.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Vertex.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "CommonShapes.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


class Mesh {
public:
	GLuint id = 0;

    // Constructor for vector (dynamic size)
	Mesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds)
		: vertices(verts), indices(inds) {
		RecalculateMesh();
	}

	Mesh(const Shapes::Shape& shape)
		: vertices(shape.getVertices()), indices(shape.getIndices()) {
		RecalculateMesh();
	}

    // Constructor for array (fixed size)
	template <size_t NVerts, size_t NInds>
	Mesh(const std::array<Vertex, NVerts>& verts, const std::array<uint32_t, NInds>& inds)
		: vertices(verts.begin(), verts.end()), indices(inds.begin(), inds.end()) {
		RecalculateMesh();
	}
	
	void RecalculateMesh();
	void Draw(Shader& shader, Camera& camera);

	~Mesh();

	Transform transform;

	std::vector<Vertex> vertices = {};
	std::vector<GLuint> indices = {};

	glm::mat4 model = {};

	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
};

#endif