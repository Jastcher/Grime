#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "mesh.h"
#include <string>

struct Graph
{
	Graph(const char* _name) : name(_name)
	{
		name.resize(16);
		mesh.Set(data);
	}

	std::string name;
	glm::vec3 color = glm::vec3(1.0f);
	std::vector<float> data {};
	float thickness = 1.0f;

	Mesh mesh;
};
