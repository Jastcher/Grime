#include "graph.h"
#include <stdio.h>
#include "GraphShader.h"

Graph::Graph(const char* _name, const std::string& _equation) : name(_name)
{
	name.reserve(16);
	mesh.Set(data);

	equation = _equation;

	// equation.reserve(16);

	shader.Init(GetEmptyGLSL().c_str(), GetEmptyGLSL().c_str(), "empty");
}

Graph::~Graph()
{
}

void Graph::SetData(const std::vector<float>& _data)
{
	data = _data;
	mesh.Set(data);
}

const std::string Graph::SetShader(std::string eq)
{
	// replace first '=' with '-'
	eq[std::find(eq.begin(), eq.end(), '=') - eq.begin()] = '-';

	std::string vertex = GetVertex();
	std::string fragment = GetFragment(eq, variables);

	std::string out = shader.Init(vertex.c_str(), fragment.c_str(), name.c_str());
	return out;
}
