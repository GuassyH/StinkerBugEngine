#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "stb/stb_image.h"

class Texture {
public:
	GLuint ID;

	int imgWidth, imgHeight, numColCh;

	Texture() : imgWidth(1), imgHeight(1), numColCh(1), ID(1) {};
	Texture(const char* fileName);
	Texture(int imgWidth, int imgHeight);
	void Delete();
};

#endif