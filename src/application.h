#pragma once
#include <iostream>

#include "camera.h"
#include "event/event.h"
#include "frameBuffer.h"
#include "graphManager.h"
#include "renderer.h"
#include "ui.h"
#include "window.h"

class Application {

public:
  Application(const Props &winProps);
  ~Application();

  void Run();

private:
  static Application *instance;
  GLFWwindow *glfwWindow;
  Window *window;
  Camera *camera;
  GraphManager *graphManager;

  Renderer *renderer;
  FrameBuffer *frameBuffer;
  UI *ui;

  // event
  Dispatcher *dispatcher;
  void HandleEvent(const Event &e);
};
