#include "ui.h"
#include "frameBuffer.h"
#include "graphManager.h"
#include "imgui.h"
#include "pfd/portable-file-dialogs.h"
#include "renderer.h"
#include <string>

static inline void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

static inline void RenderUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UI::UI(Renderer* _renderer, GraphManager* _graphManager, Window* _window, FrameBuffer* _frameBuffer, Camera* _camera)
    : renderer(_renderer), graphManager(_graphManager), window(_window), frameBuffer(_frameBuffer), camera(_camera)
{

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

	ImGui_ImplGlfw_InitForOpenGL(window->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

UI::~UI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

static inline void MainWindow()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags host_window_flags = 0;
	host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
	    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
	host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("MainWindow", nullptr, host_window_flags);
	ImGui::PopStyleVar(3);
	ImGuiID dockspace_id = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, nullptr);
	ImGui::End();
}

static inline void ViewportWindow(
    FrameBuffer* frameBuffer, Camera* camera, bool& viewportFocused, int& viewportMouseX, int& viewportMouseY)
{
	ImGui::Begin("Viewport");
	float viewportWidth = ImGui::GetWindowWidth();
	float viewportHeight = ImGui::GetWindowHeight();
	camera->width = viewportWidth;
	camera->height = viewportHeight;

	// Swap the y-coordinates in the ImVec2 to avoid rendering the framebuffer upside down
	ImGui::GetWindowDrawList()->AddImage(
	    (unsigned int*)frameBuffer->textureColorbuffer,
	    ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y),
	    ImVec2(ImGui::GetCursorScreenPos().x + viewportWidth, ImGui::GetCursorScreenPos().y + viewportHeight),
	    ImVec2(0, 1),
	    ImVec2(1, 0));

	viewportFocused = ImGui::IsWindowFocused();

	ImVec2 mousePos = ImGui::GetMousePos();
	viewportMouseX = mousePos.x - ImGui::GetCursorScreenPos().x - ImGui::GetScrollX();
	viewportMouseY = mousePos.y - ImGui::GetCursorScreenPos().y - ImGui::GetScrollY();
	// std::cout << viewportMouseX << " " << viewportMouseY << std::endl;

	ImGui::End();
}

static inline void InfoWindow(Window* window, Camera* camera, bool viewportFocused, Renderer* renderer)
{
	ImGui::Begin("Info");
	ImGui::Text("Window size: %d x %d", window->GetWidth(), window->GetHeight());
	ImGui::Text("Camera size: %d x %d", camera->width, camera->height);
	if(viewportFocused) { ImGui::Text("Viewport window is focused"); }
	ImGui::Text("Mouse pos: %f x %f", window->mouseX, window->mouseY);
	ImGui::Text("Camera Last mouse pos: %f x %f", camera->lastcamX, camera->lastcamY);
	// ImGui::InputFloat("Grid Spacing", &renderer->gridSpacing);
	ImGui::Text("dt: %f", window->dt);
	ImGui::Text("fps: %.1f", 1 / window->dt);

	ImGui::End();
}

static inline void Settings(Camera* camera, Renderer* renderer)
{
	ImGui::Begin("Settings");
	if(ImGui::TreeNode("Camera Settings"))
	{

		ImGui::Columns(2);
		// ImGui::SetColumnWidth(0, 100);

		ImGui::Dummy(ImVec2(0.0f, 1.0f));
		ImGui::Text("Camera position");
		ImGui::Dummy(ImVec2(0.0f, 2.0f));
		ImGui::Text("FOV");
		ImGui::Dummy(ImVec2(0.0f, 2.0f));
		ImGui::Text("Scroll speed");
		ImGui::Dummy(ImVec2(0.0f, 2.0f));
		ImGui::Text("FOV transition duration");
		ImGui::Dummy(ImVec2(0.0f, 2.0f));
		ImGui::Text("FOV transition speed");

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);

		ImGui::InputFloat2("##0", glm::value_ptr(camera->position));
		// update grid spacing on manual fov change
		ImGui::SetNextItemWidth(-1);
		if(ImGui::InputFloat("##1", &camera->fov))
		{
			camera->fovChangeCounter = 0;
			renderer->gridSpacing = camera->fov / 10.0f;
		}
		ImGui::SetNextItemWidth(-1);
		ImGui::InputFloat("##2", &camera->scrollSpeed);
		ImGui::SetNextItemWidth(-1);
		ImGui::InputFloat("##3", &camera->fovChangeDuration);
		ImGui::SetNextItemWidth(-1);
		ImGui::InputFloat("##4", &camera->fovTransitionSpeed);
		ImGui::TreePop();
		ImGui::Columns(1);
	}

	ImGui::Separator();

	if(ImGui::TreeNode("Grid Settings"))
	{
		ImGui::Columns(2);
		ImGui::Dummy(ImVec2(0.0f, 1.0f));
		ImGui::Text("Child grid spacing");
		ImGui::InputFloat("##5", &renderer->gridSpacing);
		ImGui::TreePop();
	}

	ImGui::End();
}

static int gridHierarchySelected = -1;
static int graphsHierarchySelected = -1;
static inline void HierarchyWindow(GraphManager* graphManager)
{
	ImGui::Begin("Hierarchy");

	if(graphManager->gridNames.size() > 0)
	{
		ImGui::Text("Grid Hierarchy");
		if(ImGui::ListBox("##0",
		                  &gridHierarchySelected,
		                  graphManager->gridNames.data(),
		                  graphManager->gridNames.size(),
		                  graphManager->gridNames.size()))
		{
			graphsHierarchySelected = -1;
		}
	}

	if(graphManager->graphNames.size() > 0)
	{
		ImGui::Text("Graphs Hierarchy");
		if(ImGui::ListBox("##1",
		                  &graphsHierarchySelected,
		                  graphManager->graphNames.data(),
		                  graphManager->graphNames.size(),
		                  graphManager->graphNames.size()))
		{
			gridHierarchySelected = -1;
		}
	}

	if(ImGui::Button("Add graph"))
	{
		auto dir =
		    pfd::open_file("select folder containing two columns of data", "/home/jastcher/Programming/c++/Grime/res/")
		        .result();
		std::cout << "opening file: ";
		for(auto& i : dir) { std::cout << i; }
		std::cout << std::endl;
		bool a = graphManager->LoadGraphCVT(dir[0].c_str());
		std::cout << "LOADED" << std::endl;
	}

	ImGui::End();
}

static inline void PropertiesWindow(GraphManager* graphManager)
{

	ImGui::Begin("Properties");
	if(gridHierarchySelected >= 0)
	{
		auto& graph = graphManager->grid[gridHierarchySelected];
		ImGui::Text("%s", graph.name.c_str());
		ImGui::ColorPicker3("Color", glm::value_ptr(graph.color));
		ImGui::InputFloat("Thickness", &graph.thickness);
	}
	if(graphsHierarchySelected >= 0)
	{
		auto& graph = graphManager->graphs[graphsHierarchySelected];
		ImGui::Text("%s", graph.name.c_str());
		ImGui::ColorPicker3("Color", glm::value_ptr(graph.color));
		ImGui::InputFloat("Thickness", &graph.thickness);

		std::cout << std::endl;
		for(auto& i : graphManager->graphs[graphsHierarchySelected].data) { std::cout << i << std::endl; }

		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
		if(ImGui::TreeNode("Vertices"))
		{

			if(ImGui::BeginTable("table1", 2, flags))
			{
				ImGui::TableSetupColumn("X");
				ImGui::TableSetupColumn("Y");

				ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
				for(int dataIndex = 0; dataIndex < graphManager->graphs[graphsHierarchySelected].data.size();
				    dataIndex += 2)
				{
					ImGui::TableNextRow();
					for(int column = 0; column < 2; column++)
					{
						ImGui::TableSetColumnIndex(column);
						if(ImGui::InputFloat(("##" + std::to_string(dataIndex + column)).c_str(),
						                     &graphManager->graphs[graphsHierarchySelected].data[dataIndex + column]),
						   ImGuiWindowFlags_NoDecoration)
						{
							graphManager->graphs[graphsHierarchySelected].mesh.Set(
							    graphManager->graphs[graphsHierarchySelected].data);
						}
					}
				}
				ImGui::EndTable();
			}
			ImGui::PopStyleColor();
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void TestingWindow(Renderer* renderer, Camera* camera)
{
	ImGui::Begin("Testing Window");
	ImGui::InputFloat("fov", &camera->fov, 0.0, 0.0, "%.9f");
	ImGui::InputFloat("spacing", &renderer->gridSpacing, 0.0, 0.0, "%.9f");
	ImGui::End();
}
void UI::Update()
{
	StartFrame();

	ImGui::SetNextWindowSize(ImVec2(window->GetWidth(), window->GetHeight()));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	// ImGui::ShowDemoWindow();

	MainWindow();
	ViewportWindow(frameBuffer, camera, viewportFocused, viewportMouseX, viewportMouseY);
	InfoWindow(window, camera, viewportFocused, renderer);
	Settings(camera, renderer);
	HierarchyWindow(graphManager);
	PropertiesWindow(graphManager);
	TestingWindow(renderer, camera);

	RenderUI();
}
