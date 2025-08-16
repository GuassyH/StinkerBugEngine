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

#include "Camera.h"
#include "Constants.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Transform;
class Material;

class Mesh {
public:
	Mesh() = default;
    // Constructor for vector (dynamic size)
	Mesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds);

	Mesh(const Constants::Shapes::Shape& shape);
    // Constructor for array (fixed size)
	template <size_t NVerts, size_t NInds>
	Mesh(const std::array<Vertex, NVerts>& verts, const std::array<uint32_t, NInds>& inds);
	
	void RecalculateMesh();
	void Draw(Camera& camera, Material* material, Transform& transform);

	~Mesh();

	std::vector<Vertex> vertices = {};
	std::vector<GLuint> indices = {};

	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
};

#endif