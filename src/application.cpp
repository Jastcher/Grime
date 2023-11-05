#include "application.h"
#include "GLFW/glfw3.h"
#include "event/event.h"
#include "glm/ext/quaternion_common.hpp"
#include "graphManager.h"
#include <cmath>
#include <iostream>

void Application::HandleEvent(const Event& e)
{

	if(e.Type() == ResizeEvent::descriptor)
	{
		const ResizeEvent& resizeEvent = static_cast<const ResizeEvent&>(e);

		// std::cout << "Window Resized; " << resizeEvent.width << " x " <<
		// resizeEvent.height << std::endl;
		// frameBuffer->Resize(resizeEvent.width, resizeEvent.height);
	}
	else if(e.Type() == MouseClickEvent::descriptor)
	{
		const MouseClickEvent& mouseClickEvent = static_cast<const MouseClickEvent&>(e);

		// std::cout << "Mouse clicked; " << mouseClickEvent.key << " " <<
		// mouseClickEvent.action << std::endl;
		window->mouseClicked = mouseClickEvent.action;
		if(mouseClickEvent.action == GLFW_RELEASE) { camera->firstClick = true; }
	}
	else if(e.Type() == MouseMoveEvent::descriptor)
	{
		const MouseMoveEvent& mouseMoveEvent = static_cast<const MouseMoveEvent&>(e);

		// std::cout << "Mouse moved; " << mouseMoveEvent.x << " " <<
		// mouseMoveEvent.y << std::endl;
		window->mouseX = mouseMoveEvent.x;
		window->mouseY = mouseMoveEvent.y;
		if(window->mouseClicked && ui->viewportFocused) { camera->Input(ui->viewportMouseX, ui->viewportMouseY); }
	}
	else if(e.Type() == ScrollEvent::descriptor)
	{
		const ScrollEvent& scrollEvent = static_cast<const ScrollEvent&>(e);

		// std::cout << "Mouse scrolled; " << scrollEvent.scrollY << std::endl;
		if(ui->viewportFocused)
		{
			camera->fovChangeCounter += scrollEvent.scrollY;
			// camera->fov -= scrollEvent.scrollY * camera->scrollSpeed * camera->fov;
			// camera->fov = (camera->fov > 0) ? camera->fov : 0.1f;
			camera->ChangeFov(camera->fov - scrollEvent.scrollY * camera->scrollSpeed * camera->fov);
			if(camera->fovChangeCounter % 5 == 0) renderer->gridSpacing = camera->newFov / 10.0f;
		}
	}
}

Application::Application(const Props& winProps)
{

	dispatcher = new Dispatcher(std::bind(&Application::HandleEvent, this, std::placeholders::_1));

	window = new Window(winProps, dispatcher);
	if(!window->Init()) { std::cout << "window failed to init" << std::endl; }
	window->EnableDepthTest(false);

	glfwWindow = window->window;

	camera = new Camera(window->GetWidth(), window->GetHeight(), 1.0f);

	renderer = new Renderer(camera);
	frameBuffer = new FrameBuffer(window);
	graphManager = new GraphManager();
	ui = new UI(renderer, graphManager, window, frameBuffer, camera);
}

Application::~Application()
{
	delete window;
	delete renderer;
	delete ui;
	delete frameBuffer;
	delete dispatcher;
	delete camera;
	delete graphManager;

	glfwTerminate();
}

void Application::Run()
{

	while(!glfwWindowShouldClose(glfwWindow))
	{
		camera->UpdateFov(window->dt);

		// same size check in method, viewport change in method
		frameBuffer->Resize(camera->width, camera->height);

		frameBuffer->Bind();

		// clear buffers
		window->Clear();

		renderer->RenderGrid();

		renderer->RenderMainAxes();

		// render graphs
		for(const auto& graph : graphManager->graphs) { renderer->Render(graph); }

		frameBuffer->Unbind();

		// ui has pointer to framebuffer
		ui->Update();

		// swap buffers and poll events
		window->Update();
	}
}
