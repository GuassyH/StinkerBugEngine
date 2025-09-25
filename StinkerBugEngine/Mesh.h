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
#include "Texture.h"
#include "Material.h"

class Transform;
class Camera;
struct Light;

struct Mesh {
public:
	const char* name = "new_mesh";

	Mesh() = default;
	// Mesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds);
	Mesh(const Constants::Shapes::Shape& shape, std::vector<Texture> textures = {});
	Mesh(std::vector<Vertex> verts, std::vector<uint32_t> inds, std::vector<Texture> textures = {});
	

	void RecalculateMesh();

	void render(Material* material, Transform* m_transform, Transform* c_transform, Camera* cam, Light* light, bool shadowPass);

	void cleanup();

	std::vector<Texture> textures = {};
	std::vector<Vertex> vertices = {};
	std::vector<GLuint> indices = {};


	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
};

#endif