#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "stb/stb_image.h"

#include <assimp/scene.h>

class Texture {
public:
	GLuint ID;
	aiTextureType type;
	std::string directory;
	std::string path;

	int imgWidth, imgHeight, numColCh;

	void Bind();
	void Generate();
	void Delete();
	void Load(bool flip, GLenum filter_type = GL_LINEAR_MIPMAP_LINEAR);

	Texture() : directory("null"), path("null"), type(aiTextureType_DIFFUSE), imgHeight(1), imgWidth(1), numColCh(4), ID(0) {};
	Texture(int imgWidth, int imgHeight);
	Texture(std::string dir, std::string path, aiTextureType type = aiTextureType_DIFFUSE) : directory(dir), path(path), type(type), imgWidth(1), imgHeight(1), numColCh(3), ID(0) {}
	
	// model loading


};

#endif