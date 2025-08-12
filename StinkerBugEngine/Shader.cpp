#include "Shader.h"
#include <fstream>     // for std::ifstream
#include <sstream>     // for std::stringstream
#include <string>      // for std::string
#include <iostream>    // for std::cerr

std::string ReadFile(const std::string& filepath) {
	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filepath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

void CheckCompileErrors(unsigned int shader, const std::string& type) {
	int success;
	char infoLog[1024];

	if (type != "PROGRAM") {
		// Shader compilation error checking
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "[ERROR] Shader Compilation Error (Type: " << type << ")\n" << infoLog << std::endl;
		}
	}
	else {
		// Shader program linking error checking
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "[ERROR] Program Linking Error\n" << infoLog << std::endl;
		}
	}
}

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) {
	std::string vertCode = ReadFile(vertexShaderFile);
	std::string fragCode = ReadFile(fragmentShaderFile);

	const char* vertSource = vertCode.c_str();
	const char* fragSource = fragCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertSource, NULL);
	glCompileShader(vertexShader);
	CheckCompileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);
	CheckCompileErrors(fragmentShader, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use() {
	glUseProgram(ID);
}

// COPIED I WANT TO LEARN HOW IT WORKS