#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <algorithm>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class VBO {
	public:
		VBO();
		unsigned int ID;
		void BindBufferData(GLsizeiptr size, const void* data);
		void Bind();
		void Unbind();
		void Delete();
};

#endif