#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include "glm/glm.hpp"
#include "Shader.h"


class Material {
public:
	bool Lit = true;
	bool Depth = true;
	bool fullscreen_pass = false;
	glm::vec4 Color = glm::vec4(0.8, 0.8, 0.8, 1.0);
	Shader shader;

	// Texture diffuse0;
	// Texture specular0;

	Material() : shader(Shader("default.vert", "default.frag")) {}
	Material(Shader& shader) : shader(shader) {}
};



#endif