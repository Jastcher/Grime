#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "mesh.h"
#include <string>

struct Graph {
    Graph(const std::string& _name, const glm::vec3& _color, const std::vector<float>& _data, float _thickness = 1.0f)
        : name(_name), color(_color), data(_data), thickness(_thickness) {

        mesh.Set(_data);
    }

    std::string name;
	glm::vec3 color;
	std::vector<float> data;
	float thickness = 1.0f;

	Mesh mesh;

};
