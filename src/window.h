#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "event/event.h"
#include <chrono>
#include <memory>

struct Props
{
	unsigned int width = 800;
	unsigned int height = 600;
	const char* title = "Grime";

	std::shared_ptr<Dispatcher> dispatcher;
};

class Window
{
  public:
	Window(const Props& properties, std::shared_ptr<Dispatcher> dispatcher);
	~Window();

	bool Init();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	const char* GetTitle() const;
	void EnableDepthTest(bool toggle) const;

	double mouseX, mouseY;
	bool mouseClicked = false;

	void SetTitle(const char* t);

	void Clear();
	void Update();

	static void ErrorCallback(int error, const char* description);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	GLFWwindow* window;

	double time;
	// time
	unsigned long long int prev =
	    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
	        .count();
	unsigned long long int now = 0;
	float dt = 1 / 165.0f;

  private:
	Props properties;
};
