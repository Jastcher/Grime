#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
ShaderProgramSource GetFileContents(const char* filename);

class Shader
{
  public:
	GLuint ID;
	Shader();
	Shader(const char* fileName);

	ShaderProgramSource LoadFile(const char* fileName);
	const std::string Init(const char* VertexSource, const char* FragmentSource, const char* fileName = "custom");

	void Activate();
	void Delete();
	void SetInt(const char* uniform, int unit);
	void SetFloat(const char* uniform, float unit);
	void SetVec2(const char* uniform, glm::vec2 unit);
	void SetVec3(const char* uniform, glm::vec3 unit);
	void SetVec4(const char* uniform, glm::vec4 unit);
	void SetMat4(const char* uniform, glm::mat4 unit);

  private:
	const std::string CompileErrors(unsigned int shader, const char* type, const char* shaderName);
};
