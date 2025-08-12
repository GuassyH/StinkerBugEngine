#include "VBO.h"


VBO::VBO() { glGenBuffers(1, &ID);	}

void VBO::Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }

void VBO::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0);	 }

void VBO::BindBufferData(GLsizeiptr size, const void* data) {
	VBO::Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VBO::Delete() { glDeleteBuffers(1, &ID);	}