
#include "frameBuffer.h"
#include "renderer.h"
#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include "graphManager.h"
#include "pfd/portable-file-dialogs.h"


class UI {

public:

    UI(Renderer* _renderer, GraphManager* graphManager, Window* _window, FrameBuffer* _frameBuffer, Camera* camera);
    ~UI();

    void Update();


    bool viewportFocused;
    int viewportMouseX, viewportMouseY;


private:

    Renderer* renderer;
    GraphManager* graphManager;
    Window* window;
    FrameBuffer* frameBuffer;
    Camera* camera;
};
