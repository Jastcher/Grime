#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"
#include "graph.h"
#include "mesh.h"
#include "shader.h"
#include <memory>
#include "window.h"

class Renderer
{
  public:
	Renderer(std::shared_ptr<Camera> cam, std::shared_ptr<Window> win);

	void Render();
	void Render(const Graph& graph);
	void RenderGPU(Graph& graph);
	void RenderGrid();
	void RenderMainAxes();

	// grid
	float gridSpacing = 0.1f;
	float parentGridSpacing = 4.0f;
	float gridThickness = 1.0f;
	float parentGridThickness = 2.0f;
	float gridMix = 0.8f;

	// axis
	float axisThickness = 1.0f;

	// grid colors
	glm::vec3 gridColor = glm::vec3(0.2f);
	glm::vec3 mainAxisXColor = glm::vec3(0.1f, 0.8f, 0.1f);
	glm::vec3 mainAxisYColor = glm::vec3(0.8f, 0.1f, 0.1f);

	GLuint lonelyVao;

  private:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Window> window;

	Shader shader = Shader("/home/jastcher/Programming/c++/Grime/src/shader/default.shader");
	Shader instanceShader = Shader("/home/jastcher/Programming/c++/Grime/src/shader/instance.shader");
	Shader gridShader = Shader("/home/jastcher/Programming/c++/Grime/src/shader/grid.shader");
	Shader axisShader = Shader("/home/jastcher/Programming/c++/Grime/src/shader/mainAxis.shader");
	Shader graphShader = Shader("/home/jastcher/Programming/c++/Grime/src/shader/graph.shader");
};
