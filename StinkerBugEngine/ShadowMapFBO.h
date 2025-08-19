#ifndef SHADOW_MAP_FBO_H
#define SHADOW_MAP_FBO_H

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <iostream>

class ShadowMapFBO {
public:
	ShadowMapFBO() = default;
	~ShadowMapFBO() = default;

	bool Init(unsigned int width, unsigned int height);
	void BindForWriting();
	void BindForReading(GLenum TextureUnit);
private:
	unsigned int m_width = 0;
	unsigned int m_height = 0;
	GLuint m_fbo;
	GLuint m_shadowMap;
}; 


#endif 