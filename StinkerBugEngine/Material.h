#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include "glm/glm.hpp"
#include "Shader.h"



enum MaterialFlags_ {
	MaterialFlags_None = 0,
	MaterialFlags_Lit = 1 << 1,
	MaterialFlags_Depth = 1 << 2,
	MaterialFlags_NoDepthTest = 1 << 3,
	MaterialFlags_Shadow = 1 << 4,
	MaterialFlags_FullscreenPass = 1 << 5
};

class Material {
public:
	glm::vec4 Color = glm::vec4(0.8, 0.8, 0.8, 1.0);
	Shader shader;
	uint32_t flags;


	// Texture diffuse0;
	// Texture specular0;

	bool HasFlag(uint32_t flag) {
		return (flags & flag) != 0;
	}
	void RemoveFlag(uint32_t flag) {
		flags &= ~flag;
	}
	void AddFlag(uint32_t flag) {
		flags |= flag;
	}

	void Recompile() {
		shader.Recompile(this);
	}

	Material() {
		flags = 0;
		shader = Shader("default.vert", "default.frag", this);
	}
	Material(uint32_t new_flags) {
		flags = new_flags;
		shader = Shader("default.vert", "default.frag", this);
	}
	Material(Shader& shader) : flags(0), shader(shader) {
		shader.Recompile(this);
	}
	Material(Shader& shader, uint32_t new_flags) : flags(new_flags), shader(shader) {
		shader.Recompile(this);
	}
	~Material() = default;
};



#endif