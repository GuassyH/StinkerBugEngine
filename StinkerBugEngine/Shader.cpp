#include "Shader.h"
#include <fstream>     // for std::ifstream
#include <sstream>     // for std::stringstream
#include <string>      // for std::string
#include <iostream>    // for std::cerr

// READFILE IS BORROWED
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

// Should be a template maybe?
Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) {
	std::string vertCode = ReadFile(vertexShaderFile);
	std::string fragBaseCode = ReadFile(fragmentShaderFile);

	std::ostringstream defStr; defStr << "#version 460 core" << "\n";
	
	defStr << "#define LIT" << "\n";
	defStr << "#define DEPTH" << "\n";

	std::string fragCode = defStr.str() + "\n//" + fragBaseCode;
	// std::cout << fragCode << std::endl;

	const char* vertSource = vertCode.c_str();
	const char* fragSource = fragCode.c_str();

	Compile(vertSource, fragSource);
}


void Shader::Compile(const char* vertSource, const char* fragSource) {

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
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(ID);
}

