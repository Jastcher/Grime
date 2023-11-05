#include "renderer.h"
#include <cmath>

template<class T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
	os << "[";
	for(typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii) { os << " " << *ii; }
	os << "]";
	return os;
}

Renderer::Renderer(Camera* cam) : camera(cam)
{
	glGenVertexArrays(1, &lonelyVao);
}

void Renderer::RenderGrid()
{
	gridShader.Activate();
	gridShader.SetMat4("view", camera->GetView());
	gridShader.SetMat4("proj", camera->GetProjection());
	gridShader.SetFloat("gridSpacing", 1 / gridSpacing);
	glBindVertexArray(lonelyVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Renderer::RenderMainAxes()
{

	axisShader.Activate();
	axisShader.SetMat4("view", camera->GetView());
	axisShader.SetMat4("proj", camera->GetProjection());
	axisShader.SetVec3("Xcolor", glm::vec3(0.8f, 0.1f, 0.1f));
	axisShader.SetVec3("Ycolor", glm::vec3(0.1f, 0.8f, 0.1f));
	glBindVertexArray(lonelyVao);
	glDrawArrays(GL_LINES, 0, 4);
}

void Renderer::Render(const Mesh& mesh)
{
	mesh.Activate();
	glDrawArrays(GL_LINE_STRIP, 0, mesh.size / 2);
}

void Renderer::Render(const Graph& graph)
{
	shader.Activate();
	shader.SetVec3("color", graph.color);
	shader.SetMat4("camera", camera->GetMatrix());
	glLineWidth(graph.thickness);

	Render(graph.mesh);
}

void Renderer::RenderInstance(const Graph& graph, const glm::vec2& offset, const glm::vec2& margin, unsigned int count)
{
	instanceShader.Activate();
	instanceShader.SetVec2("offset", offset);
	instanceShader.SetVec2("margin", margin);
	instanceShader.SetVec3("color", graph.color);
	instanceShader.SetMat4("camera", camera->GetMatrix());

	glm::mat4 model = glm::mat4(1.0f);
	instanceShader.SetMat4("model", model);

	glLineWidth(graph.thickness);

	graph.mesh.Activate();

	glDrawArraysInstanced(GL_LINES, 0, 2, count);
}
