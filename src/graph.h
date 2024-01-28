#pragma once

#include "glm/glm.hpp"
#include "mesh.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "tinyexpr/tinyexpr.h"

class Graph
{
  public:
	Graph(const char* _name) : name(_name)
	{
		name.reserve(16);
		mesh.Set(data);

		equation.reserve(16);
	}

	~Graph() {}

	void SetData(const std::vector<float>& _data)
	{
		data = _data;
		mesh.Set(data);
	}

	std::string name;
	glm::vec3 color = glm::vec3(1.0f);
	std::vector<float> data {};
	float thickness = 1.0f;

	bool isLoaded = false; // if we intend to not use equation and load the data instead

	std::unordered_map<std::string, double> variables;

	std::string equation;
	te_parser parser;

	float increment = 0.1f;
	float leftMostX = 0.0f;
	float rightMostX = 0.0f;

	Mesh mesh;
};
