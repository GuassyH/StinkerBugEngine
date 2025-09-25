#include "Shader.h"
#include <fstream>     // for std::ifstream
#include <sstream>     // for std::stringstream
#include <string>      // for std::string
#include <iostream>    // for std::cerr

#include "Material.h"

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
Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile, Material* material) : vertexPath(vertexShaderFile), fragmentPath(fragmentShaderFile) {
	std::string vertCode = ReadFile(vertexShaderFile);
	std::string fragBaseCode = ReadFile(fragmentShaderFile);

	std::ostringstream defStr; defStr << "#version 460 core" << "\n";
	
	if(!material) {
		defStr << "#define LIT" << "\n";
		defStr << "#define DEPTH" << "\n";
		defStr << "#define SHADOW" << "\n";
	}
	else {
		if(material->HasFlag(MaterialFlags_Lit)) {
			defStr << "#define LIT" << "\n";
		}
		if(material->HasFlag(MaterialFlags_Depth)) {
			defStr << "#define DEPTH" << "\n";
		}
		if(material->HasFlag(MaterialFlags_Shadow)) {
			defStr << "#define SHADOW" << "\n";
		}
		if (material->HasFlag(MaterialFlags_NoDepthTest)) {
			defStr << "#define NO_DEPTH_TEST" << "\n";
		}
	}

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

void Shader::Recompile(Material* material) {
	std::string vertCode = ReadFile(vertexPath);
	std::string fragBaseCode = ReadFile(fragmentPath);

	std::ostringstream defStr; defStr << "#version 460 core" << "\n";


	if (!material) {
		defStr << "#define LIT" << "\n";
		defStr << "#define DEPTH" << "\n";
		defStr << "#define SHADOW" << "\n";
	}
	else {
		if (material->HasFlag(MaterialFlags_Lit)) {
			defStr << "#define LIT" << "\n";
		}
		if (material->HasFlag(MaterialFlags_Depth)) {
			defStr << "#define DEPTH" << "\n";
		}
		if (material->HasFlag(MaterialFlags_Shadow)) {
			defStr << "#define SHADOW" << "\n";
		}
		if (material->HasFlag(MaterialFlags_NoDepthTest)) {
			defStr << "#define NO_DEPTH_TEST" << "\n";
		}
	}

	std::string fragCode = defStr.str() + "\n//" + fragBaseCode;
	// std::cout << fragCode << std::endl;

	const char* vertSource = vertCode.c_str();
	const char* fragSource = fragCode.c_str();

	glDeleteProgram(ID); 

	Compile(vertSource, fragSource);

}

void Shader::Use() {
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}

#pragma region HELPER FUNCTIONS

void Shader::SetBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}
void Shader::SetVec2(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.a);
}
void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const {
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const {
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

#pragma endregion
