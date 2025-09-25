#include "Texture.h"


// Normal Texture
Texture::Texture(int width, int height) : directory("null"), path("null"), type(aiTextureType_DIFFUSE), imgHeight(height), imgWidth(width), numColCh(8), ID(0) {
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, imgWidth, imgHeight, 0, GL_RGB8, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture::Texture(std::string dir, std::string path, aiTextureType type) : directory(dir), path(path), type(type), imgWidth(1), imgHeight(1), numColCh(1), ID(0) {}

void Texture::Bind() {
	glActiveTexture(ID);
}

void Texture::Generate() {
	glGenTextures(1, &ID);
}

void Texture::Load(bool flip) {
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* bytes = stbi_load((directory + "/" + path).c_str(), &imgWidth, &imgHeight, &numColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ID);

	GLenum colorMode = GL_RGB;
	switch (numColCh)
	{
	case 1:
		colorMode = GL_RED;
		break;
	case 2:
		colorMode = GL_RGBA;
		break;
	}

	if (bytes) {

		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, imgWidth, imgHeight, 0, colorMode, GL_UNSIGNED_BYTE, bytes);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}