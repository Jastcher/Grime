#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <vector>


class Mesh {
public:
    Mesh();
	void Set(const std::vector<float>& vertices);

	void Activate() const;

	int size;

private:
	unsigned int VAO;
    unsigned int VBO;
};
