#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <iostream>
#include <vector>
#include <array>

#include "Vertex.h"

#include "Constants.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"

class Transform;
class Material;
class Camera;
struct Light;


struct Mesh {
public:
	const char* name = "new_mesh";

	Mesh() = default;
	template <size_t NVerts, size_t NInds>
	Mesh(const std::array<Vertex, NVerts>& verts, const std::array<uint32_t, NInds>& inds);
	Mesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds);
	Mesh(const Constants::Shapes::Shape& shape);
	

	void RecalculateMesh();

	void render(Material* m_material, Transform* m_transform, Transform* c_transform, Camera* cam, Light* light, bool shadowPass);

	~Mesh();

	std::vector<Vertex> vertices = {};
	std::vector<GLuint> indices = {};


	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
};

#endif