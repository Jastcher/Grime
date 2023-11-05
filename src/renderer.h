#pragma once
#include "glad/glad.h" // ERROR CAUSED BY NVIM FORMATTER, THIS INCLUDE SHOULD BE BEFORE GLFW INCLUDE
#include "GLFW/glfw3.h"
#include "camera.h"
#include "graph.h"
#include "mesh.h"
#include "shader.h"

class Renderer
{
  public:
	Renderer(Camera* camera);

	void Render();
	void Render(const Mesh& mesh);
	void Render(const Mesh& mesh, glm::vec3 color);
	void Render(const Graph& graph);
	void RenderInstance(const Graph& graph, const glm::vec2& offset, const glm::vec2& margin, unsigned int count);
	void RenderGrid();
	void RenderMainAxes();

	float gridSpacing = 0.1f;
	GLuint lonelyVao;

  private:
	Camera* camera;

	Shader shader = Shader("/home/jastcher/Programming/c++/Grime/src/shader/default.shader");
	Shader instanceShader = Shader("/home/jastcher/Programming/c++/Grime/src/shader/instance.shader");
	Shader gridShader = Shader("/home/jastcher/Programming/c++/Grime/src/shader/grid.shader");
	Shader axisShader = Shader("/home/jastcher/Programming/c++/Grime/src/shader/mainAxis.shader");
};
