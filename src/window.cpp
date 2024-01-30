#include "window.h"
#include "event/event.h"
#include <GL/gl.h>
#include <iostream>

bool Window::Init()
{
	if(!glfwInit())
	{
		std::cout << "glfw init failed" << std::endl;
		glfwTerminate();
		return 0;
	}

	window = glfwCreateWindow(properties.width, properties.height, properties.title, NULL, NULL);
	if(!window)
	{
		std::cout << "window failed to create" << std::endl;
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(window);

	glfwSetErrorCallback(ErrorCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 16);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to init glad" << std::endl;
		return 0;
	}

	glViewport(0, 0, properties.width, properties.height);

	glfwSetWindowUserPointer(window, &properties);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	return 1;
}

Window::Window(const Props& p, std::shared_ptr<Dispatcher> disp)
{
	properties.width = p.width;
	properties.height = p.height;
	properties.title = p.title;
	properties.dispatcher = disp;
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

unsigned int Window::GetWidth() const
{
	return properties.width;
}

unsigned int Window::GetHeight() const
{
	return properties.height;
}

const char* Window::GetTitle() const
{
	return properties.title;
}

void Window::SetTitle(const char* t)
{
	properties.title = t;
}

void Window::EnableDepthTest(bool toggle) const
{
	if(toggle)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void Window::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update()
{
	// update time
	now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
	          .count();
	dt = now - prev;
	dt /= 1000;
	prev = now;

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::ErrorCallback(int error, const char* description)
{
	std::cerr << "[ERROR] " << error << " -> " << description << std::endl;
}

void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	MouseClickEvent event;
	event.key = button;
	event.action = action;

	Props& properties = *(Props*)glfwGetWindowUserPointer(window);
	properties.dispatcher->Post(event);
}

void Window::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	MouseMoveEvent event;
	event.x = xpos;
	event.y = ypos;

	Props& properties = *(Props*)glfwGetWindowUserPointer(window);
	properties.dispatcher->Post(event);
}

void Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	ResizeEvent event;
	event.width = width;
	event.height = height;

	Props& properties = *(Props*)glfwGetWindowUserPointer(window);
	properties.dispatcher->Post(event);

	properties.width = width;
	properties.height = height;
}

void Window::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ScrollEvent event;
	event.scrollX = xoffset;
	event.scrollY = yoffset;

	Props& properties = *(Props*)glfwGetWindowUserPointer(window);
	properties.dispatcher->Post(event);
}
