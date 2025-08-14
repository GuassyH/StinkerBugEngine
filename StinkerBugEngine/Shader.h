#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <algorithm>
#include <iostream>


#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Shader {
	public:
		unsigned int ID;
		Shader() = default;
		Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
		// Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
		void Compile(const char* vertSource, const char* fragSource);
		void Use();
};

#endif