#include "EBO.h"


EBO::EBO() { glGenBuffers(1, &ID);	}

void EBO::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); };
void EBO::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };

void EBO::BindBufferData(GLsizeiptr size, const void* data) {
	EBO::Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void EBO::Delete() { glDeleteBuffers(1, &ID); }