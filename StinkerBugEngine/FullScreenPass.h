#ifndef FULLSCREEN_PASS_H
#define FULLSCREEN_PASS_H

#include "Camera.h"
#include "Vertex.h"
#include "Constants.h"
#include "Material.h"
#include "Display.h"

#include "glm/glm.hpp"
#include "glad/glad.h"


#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


class FullScreenPass {
public:
	std::vector<Vertex> vertices = Constants::Shapes::Quad().getVertices();
	std::vector<GLuint> indices = Constants::Shapes::Quad().getIndices();

	FullScreenPass(Camera& camera, Material& material);
	void Draw(Camera& camera);

	VAO VAO1;
	VBO VBO1;
	EBO EBO1;

	Material* material;
};


#endif