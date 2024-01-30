
#include "frameBuffer.h"
#include "renderer.h"
#include <functional>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include "ui_styles.h"

#include "GLFW/glfw3.h"

#include "window.h"
#include "graphManager.h"

#include "pfd/portable-file-dialogs.h"
#include <unordered_map>

class UI
{

  public:
	UI(std::shared_ptr<Renderer> _renderer,
	   std::shared_ptr<GraphManager> graphManager,
	   std::shared_ptr<Window> _window,
	   std::shared_ptr<FrameBuffer> _frameBuffer,
	   std::shared_ptr<Camera> camera);

	~UI();

	void Update();

	// if mouse is hovering the viewport
	bool viewportFocused;

	// viewport mouse coords
	int viewportMouseX, viewportMouseY;

	// themes
	void (*(styleFunctions[5]))() { CleanDarkRed, Moonlight, VisualStudio, Unreal, DeepDark };
	const char* styleNames[5] { "Clean dark red", "Moonlight", "Visual Studio", "Unreal", "Deep Dark" };

	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<GraphManager> graphManager;
	std::shared_ptr<Window> window;
	std::shared_ptr<FrameBuffer> frameBuffer;
	std::shared_ptr<Camera> camera;
};
