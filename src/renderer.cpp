#include "renderer.h"
#include "glm/ext/quaternion_common.hpp"
#include "glm/matrix.hpp"
#include "graph.h"
#include <cmath>

template<class T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
	os << "[";
	for(typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii) { os << " " << *ii; }
	os << "]";
	return os;
}

Renderer::Renderer(std::shared_ptr<Camera> cam, std::shared_ptr<Window> win) : camera(cam), window(win)
{
	glGenVertexArrays(1, &lonelyVao);
}

void Renderer::RenderGPU(Graph& graph)
{
	graph.shader.Activate();
	graph.shader.SetVec3("graphColor", graph.color);
	graph.shader.SetFloat("thickness", graph.thickness);
	graph.shader.SetVec2("resolution", glm::vec2(camera->width, camera->height));
	graph.shader.SetFloat("fov", camera->fov);
	graph.shader.SetVec2("translation", camera->position);
	graph.shader.SetMat4("view", (camera->GetView()));
	graph.shader.SetMat4("proj", (camera->GetProjection()));

	graph.shader.SetFloat("time", window->time);

	for(const auto& var : graph.variables) { graph.shader.SetFloat(var.first.c_str(), var.second); }

	glBindVertexArray(lonelyVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Renderer::Render(const Graph& graph)
{
	shader.Activate();
	shader.SetMat4("camera", camera->GetMatrix());
	shader.SetVec3("color", graph.color);

	graph.mesh.Activate();
	glDrawArrays(GL_LINE_STRIP, 0, graph.mesh.size / 2);
}

void Renderer::RenderGrid()
{
	gridShader.Activate();

	gridShader.SetMat4("view", camera->GetView());
	gridShader.SetMat4("proj", camera->GetProjection());

	gridShader.SetFloat("gridSpacing", 1 / gridSpacing);
	gridShader.SetFloat("parentGridSpacing", parentGridSpacing);
	gridShader.SetFloat("gridThickness", gridThickness);
	gridShader.SetFloat("parentGridThickness", parentGridThickness);
	gridShader.SetFloat("gridMix", gridMix);

	gridShader.SetVec3("color", gridColor);

	glBindVertexArray(lonelyVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Renderer::RenderMainAxes()
{
	axisShader.Activate();

	axisShader.SetMat4("view", camera->GetView());
	axisShader.SetMat4("proj", camera->GetProjection());
	axisShader.SetVec3("Xcolor", mainAxisXColor);
	axisShader.SetVec3("Ycolor", mainAxisYColor);

	glLineWidth(axisThickness);

	glBindVertexArray(lonelyVao);
	glDrawArrays(GL_LINES, 0, 4);
}
