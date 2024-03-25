#pragma once

#include "glm/glm.hpp"
#include "mesh.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "tinyexpr/tinyexpr.h"
#include "shader.h"

enum class CalcMode
{
	CPU = 0,
	GPU,
};

class Graph
{
  public:
	Graph(const char* _name, const std::string& _equation = "");

	~Graph();

	void SetData(const std::vector<float>& _data);

	const std::string SetShader(std::string eq);

	std::string name;
	glm::vec3 color = glm::vec3(1.0f);
	std::vector<float> data {};
	float thickness = 1.0f;

	bool isLoaded = false; // if we intend to not use equation and load the data instead

	std::unordered_map<std::string, double> variables;

	std::string equation;

	CalcMode mode = CalcMode::GPU;

	// CPU rendering
	te_parser parser;

	// GPU rendering
	Shader shader;

	float increment = 0.1f;
	float leftMostX = 0.0f;
	float rightMostX = 0.0f;

	Mesh mesh;
};
