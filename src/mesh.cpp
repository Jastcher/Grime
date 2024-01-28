#include "mesh.h"
#include <iostream>

Mesh::Mesh()
{
	// gen VAO
	glGenVertexArrays(1, &VAO);

	// gen VBO
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	glEnableVertexAttribArray(0);

	// unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Set(const std::vector<float>& vertices)
{
	size = vertices.size();

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Activate() const
{
	glBindVertexArray(VAO);
}
