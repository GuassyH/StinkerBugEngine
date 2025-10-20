#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include "glm/glm.hpp"
#include "Shader.h"
#include "Renderer.h"

enum MaterialFlags_ {
	MaterialFlags_None = 0,
	MaterialFlags_Lit = 1 << 1,
	MaterialFlags_NoDepthTest = 1 << 2,
	MaterialFlags_Shadow = 1 << 3,
	MaterialFlags_FullscreenPass = 1 << 4,
	MaterialFlags_Transparent = 1 << 5,
};

class Material {
public:
	glm::vec4 color = glm::vec4(1.0);
	Shader shader;
	uint32_t flags;

	// Texture diffuse0;
	// Texture specular0;

	bool HasFlag(uint32_t flag) {
		return (flags & flag) != 0;
	}
	void RemoveFlag(uint32_t flag) {
		flags &= ~flag;
		if ((flag & MaterialFlags_Transparent) == MaterialFlags_Transparent) { // If one of the flags added is transparency, queue a rebuild
			Renderer::getInstance().queue_mesh_rebuild = true;
		}
	}
	void AddFlag(uint32_t flag) {
		flags |= flag;
		if ((flag & MaterialFlags_Transparent) == MaterialFlags_Transparent) { // If one of the flags added is transparency, queue a rebuild
			Renderer::getInstance().queue_mesh_rebuild = true;
		}
	}

	void Recompile() {
		shader.Recompile(this);
	}

	void Use() {
		shader.Use();
	}

	Material(uint32_t new_flags = 0) {
		flags = new_flags;
		shader = Shader("default.vert", "default.frag", this);
	}
	Material(Shader& shader, uint32_t new_flags = 0) : shader(shader) {
		flags = new_flags;
		shader.Recompile(this);
	}
	~Material() = default;
};



#endif