#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <algorithm>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material;

class Shader {
	public:
		std::string vertexPath;
		std::string fragmentPath;

		unsigned int ID;
		Shader() = default;
		Shader(const char* vertexShaderFile, const char* fragmentShaderFile, Material* material = nullptr);
		// Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
		void Compile(const char* vertSource, const char* fragSource);
		void Recompile(Material* material);
		void Use();

		void Delete();

		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetVec2(const std::string& name, const glm::vec2& value) const;
		void SetVec2(const std::string& name, float x, float y) const;
		void SetVec3(const std::string& name, const glm::vec3& value) const;
		void SetVec3(const std::string& name, float x, float y, float z) const;
		void SetVec4(const std::string& name, const glm::vec4& value) const;
		void SetVec4(const std::string& name, float x, float y, float z, float w);
		void SetMat2(const std::string& name, const glm::mat2& mat) const;
		void SetMat3(const std::string& name, const glm::mat3& mat) const;
		void SetMat4(const std::string& name, const glm::mat4& mat) const;
};

#endif