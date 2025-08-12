#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <algorithm>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Shader {
	public:
		unsigned int ID;
		Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
		void Use();
};

#endif