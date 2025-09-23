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
	void Load(bool flip);

	Texture() : imgWidth(1), imgHeight(1), numColCh(1), ID(1) {};
	Texture(const char* fileName);
	Texture(int imgWidth, int imgHeight);
	
	// model loading
	Texture(std::string dir, std::string path, aiTextureType type);


};

#endif