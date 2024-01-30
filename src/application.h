#pragma once
// #include <iostream>
#include <memory>

#include "camera.h"
#include "event/event.h"
#include "frameBuffer.h"
#include "graphManager.h"
#include "renderer.h"
#include "ui.h"
#include "window.h"

class Application
{

  public:
	Application(const Props& winProps);
	~Application();

	void Run();

  private:
	static Application* instance;
	GLFWwindow* glfwWindow;

	std::shared_ptr<Window> window;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<GraphManager> graphManager;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<FrameBuffer> frameBuffer;
	std::shared_ptr<UI> ui;

	// event
	std::shared_ptr<Dispatcher> dispatcher;
	void HandleEvent(const Event& e);
};
