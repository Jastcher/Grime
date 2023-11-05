#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include <iostream>

class FrameBuffer
{
  public:
	unsigned int id;
	unsigned int textureColorbuffer;
	unsigned int rbo;

	int oldWidth;
	int oldHeight;

	FrameBuffer(Window* window);
	void Resize(int width, int height);
	void SetSize(int width, int height);

	void Bind();
	void Unbind();
};
