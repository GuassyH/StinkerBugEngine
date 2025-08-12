#include "VAO.h"

VAO::VAO() { glGenVertexArrays(1, &ID);	}


void VAO::Bind() {	glBindVertexArray(ID);	}

void VAO::Unbind() {	glBindVertexArray(0);	}

void VAO::LinkAttrib(GLuint location, GLuint numElements, GLenum type, GLsizeiptr stride, GLuint offset) {
	VAO::Bind();
	glVertexAttribPointer(location, numElements, type, GL_FALSE, stride, (void*)offset);
	glEnableVertexAttribArray(location);
}


void VAO::Delete() {	glDeleteBuffers(1, &ID);	}