#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <algorithm>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class VAO {
	public:
		VAO();
		unsigned int ID;
		void LinkAttrib(GLuint location, GLuint numElements, GLenum type, GLsizeiptr stride, GLuint offset);
		void Bind();
		void Unbind();
		void Delete();
};

#endif