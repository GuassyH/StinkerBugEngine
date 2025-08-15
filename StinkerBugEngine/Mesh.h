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

#include "SceneManager.h"

#include "Vertex.h"
#include "Transform.h"
#include "Material.h"
#include "Camera.h"
#include "Constants.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


class Mesh {
public:
	GLuint id = 0;

	Mesh() = default;
    // Constructor for vector (dynamic size)
	Mesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds, Material& material);

	Mesh(const Constants::Shapes::Shape& shape, Material& material);
    // Constructor for array (fixed size)
	template <size_t NVerts, size_t NInds>
	Mesh(const std::array<Vertex, NVerts>& verts, const std::array<uint32_t, NInds>& inds, Material& material);
	
	void RecalculateMesh();
	void Draw(Camera& camera);

	~Mesh();

	Transform transform;

	std::vector<Vertex> vertices = {};
	std::vector<GLuint> indices = {};

	glm::mat4 model = {};

	Material* material = nullptr;

	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
};

#endif