#include "ui.h"
#include "frameBuffer.h"
#include "glm/common.hpp"
#include "graph.h"
#include "graphManager.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "pfd/portable-file-dialogs.h"
#include "renderer.h"
#include <string>

#define DUMMYTEXT(x) \
	ImGui::Text(x);  \
	ImGui::Dummy(ImVec2(0.0f, 2.0f));

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

UI::UI(std::shared_ptr<Renderer> _renderer,
       std::shared_ptr<GraphManager> _graphManager,
       std::shared_ptr<Window> _window,
       std::shared_ptr<FrameBuffer> _frameBuffer,
       std::shared_ptr<Camera> _camera)
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

	styleFunctions[1]();
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

static inline void ViewportWindow(UI* UI)
{
	ImGui::Begin("Viewport");
	float viewportWidth = ImGui::GetWindowWidth();
	float viewportHeight = ImGui::GetWindowHeight();
	UI->camera->width = viewportWidth;
	UI->camera->height = viewportHeight;

	// Swap the y-coordinates in the ImVec2 to avoid rendering the framebuffer upside down
	ImGui::GetWindowDrawList()->AddImage(
	    (unsigned int*)UI->frameBuffer->textureColorbuffer,
	    ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y),
	    ImVec2(ImGui::GetCursorScreenPos().x + viewportWidth, ImGui::GetCursorScreenPos().y + viewportHeight),

	    ImVec2(0, 1),
	    ImVec2(1, 0));

	UI->viewportFocused = ImGui::IsWindowHovered();

	ImVec2 mousePos = ImGui::GetMousePos();
	UI->viewportMouseX = mousePos.x - ImGui::GetCursorScreenPos().x - ImGui::GetScrollX();
	UI->viewportMouseY = mousePos.y - ImGui::GetCursorScreenPos().y - ImGui::GetScrollY();
	// std::cout << viewportMouseX << " " << viewportMouseY << std::endl;

	ImGui::End();
}

static inline void InfoWindow(UI* UI)
{
	auto* camera = UI->camera.get();
	auto* window = UI->window.get();

	ImGui::Begin("Info");
	ImGui::Text("time%f ", window->time);
	ImGui::Text("Camera bounds %f ; %f", camera->leftBound, camera->rightBound);
	ImGui::Text("Window size: %d x %d", window->GetWidth(), window->GetHeight());
	ImGui::Text("Camera size: %d x %d", camera->width, camera->height);
	if(UI->viewportFocused) { ImGui::Text("Viewport window is focused"); }
	ImGui::Text("Mouse pos: %f x %f", window->mouseX, window->mouseY);
	ImGui::Text("Camera Last mouse pos: %f x %f", camera->lastcamX, camera->lastcamY);
	// ImGui::InputFloat("Grid Spacing", &renderer->gridSpacing);
	ImGui::Text("dt: %f", window->dt);
	ImGui::Text("fps: %.1f", 1 / window->dt);

	ImGui::End();
}

static int themeSelected = 1; // Moonlight
static inline void Settings(UI* UI)
{
	auto* camera = UI->camera.get();
	auto* renderer = UI->renderer.get();
	auto* graphManager = UI->graphManager.get();

	ImGui::Begin("Settings");
	if(ImGui::TreeNode("Camera Settings"))
	{
		ImGui::Columns(2);

		ImGui::Dummy(ImVec2(0.0f, 1.0f));
		DUMMYTEXT("Camera position");
		DUMMYTEXT("FOV");
		DUMMYTEXT("Scroll speed");
		DUMMYTEXT("FOV transition duration");
		DUMMYTEXT("FOV transition speed");

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
		DUMMYTEXT("Grid spacing");
		DUMMYTEXT("Parent grid spacing");
		DUMMYTEXT("Grid thickness");
		DUMMYTEXT("Parent grid thickness");
		DUMMYTEXT("Axis thickness");
		DUMMYTEXT("Grid mix");
		DUMMYTEXT("Grid color");
		DUMMYTEXT("X axis color");
		DUMMYTEXT("Y axis color");

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		ImGui::DragFloat("##5", &renderer->gridSpacing, 0.01f);
		ImGui::SetNextItemWidth(-1);
		ImGui::DragFloat("##6", &renderer->parentGridSpacing, 1.0f);
		ImGui::SetNextItemWidth(-1);
		ImGui::DragFloat("##7", &renderer->gridThickness, 0.01f);
		ImGui::SetNextItemWidth(-1);
		ImGui::DragFloat("##8", &renderer->parentGridThickness, 0.01f);
		ImGui::SetNextItemWidth(-1);
		ImGui::DragFloat("##13", &renderer->axisThickness, 1.0f);
		ImGui::SetNextItemWidth(-1);
		ImGui::DragFloat("##9", &renderer->gridMix, 0.01f, 0.0f, 1.0f);

		ImGui::SetNextItemWidth(-1);
		ImGui::ColorEdit3("##10", glm::value_ptr(renderer->gridColor));
		ImGui::SetNextItemWidth(-1);
		ImGui::ColorEdit3("##11", glm::value_ptr(renderer->mainAxisXColor));
		ImGui::SetNextItemWidth(-1);
		ImGui::ColorEdit3("##12", glm::value_ptr(renderer->mainAxisYColor));
		ImGui::TreePop();
		ImGui::Columns(1);
	}

	ImGui::Separator();
	if(ImGui::TreeNode("Graph Settings"))
	{
		ImGui::Columns(2);
		ImGui::Dummy(ImVec2(0.0f, 1.0f));
		DUMMYTEXT("Increment");

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		if(ImGui::DragFloat("##13", &graphManager->increment, 0.001f, 0.01f))
		{
			graphManager->increment = glm::max(graphManager->increment, 0.01f);

			graphManager->UpdateAllGraphVertices(camera->leftBound, camera->rightBound, true);
		}

		ImGui::TreePop();
		ImGui::Columns(1);
	}

	ImGui::Separator();

	if(ImGui::TreeNode("UI settings"))
	{
		ImGui::Columns(2);
		DUMMYTEXT("Themes")
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		if(ImGui::ListBox("##14", &themeSelected, UI->styleNames, 5)) { UI->styleFunctions[themeSelected](); }
		ImGui::TreePop();
		ImGui::Columns(1);
	}
	ImGui::End();
}

static int graphsHierarchySelected = -1;
static inline void HierarchyWindow(UI* UI)
{
	auto* graphManager = UI->graphManager.get();

	ImGui::Begin("Hierarchy");

	std::string eq = "Null";

	// if button clicked open popup
	if(ImGui::Button("Add New Graph", ImVec2(-1.0f, 40.0f))) { ImGui::OpenPopup("Default_selection"); }

	if(ImGui::BeginPopup("Default_selection"))
	{
		ImGui::SeparatorText("Select Preset");

		if(ImGui::Selectable("Empty")) eq = "";

		if(ImGui::BeginMenu("Shapes"))
		{
			if(ImGui::Selectable("Circle")) eq = "pow((x-n), 2) + pow((y-m), 2) = pow(r,2)";
			if(ImGui::Selectable("Ellipse")) eq = "(pow(x-h,2))/pow(b,2) + (pow(y-v,2))/pow(a,2) = 1";
			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Functions"))
		{
			if(ImGui::Selectable("Linear")) eq = "y = k * x + q";
			if(ImGui::Selectable("Quadratic")) eq = "y = pow(x,2)";
			if(ImGui::Selectable("Cubic")) eq = "y = pow(x,3)";
			if(ImGui::Selectable("Exponential")) eq = "y = exp(x)";
			if(ImGui::Selectable("Logarithmic")) eq = "y = log(x)";
			if(ImGui::Selectable("Square root")) eq = "y = sqrt(x)";
			if(ImGui::Selectable("Sine")) eq = "y = sin(x)";
			if(ImGui::Selectable("Cosine")) eq = "y = cos(x)";
			if(ImGui::Selectable("Tangent")) eq = "y = tan(x)";
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}

	if(eq != "Null")
	{
		Graph* newGraph = new Graph("New graph", eq);
		if(eq != "")
		{
			graphManager->AddVariables(*newGraph);
			newGraph->SetShader(eq);
		}
		graphManager->AddGraph(newGraph);
	}

	ImGui::Separator();

	if(graphManager->graphs.size() == 0) { graphsHierarchySelected = -1; }

	for(size_t i = 0; i < graphManager->graphs.size(); i++)
	{
		std::string label = graphManager->graphs[i]->name;
		if(ImGui::Selectable((label + "##" + std::to_string(i)).c_str(),
		                     (int)i == graphsHierarchySelected,
		                     0,
		                     ImVec2(ImGui::GetWindowWidth() - 69, 0.0))) // nice
		{
			graphsHierarchySelected = i;
		}
		ImGui::SameLine(ImGui::GetWindowWidth() - 55);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
		if(ImGui::Button(("x##" + std::to_string(i)).c_str()))
		{
			graphManager->DeleteGraph(i);
			if((int)i < graphsHierarchySelected || graphsHierarchySelected == (int)graphManager->graphs.size())
				graphsHierarchySelected -= 1;
		}
	}

	ImGui::End();
}

static std::string errorOut;
static inline void PropertiesWindow(UI* UI)
{
	auto* camera = UI->camera.get();
	auto* graphManager = UI->graphManager.get();

	ImGui::Begin("Properties");
	if(graphsHierarchySelected >= 0)
	{
		auto& graph = *graphManager->graphs[graphsHierarchySelected];

		ImGui::PushItemWidth(-1);
		ImGui::Text("Name");
		ImGui::InputText("##prop0", &graph.name);

		ImGui::Separator();
		ImGui::Text("Color");
		ImGui::ColorPicker3("##prop1", glm::value_ptr(graph.color));

		ImGui::Separator();
		ImGui::Text("Thickness");
		if(ImGui::DragFloat("##prop2", &graph.thickness, camera->fov))
			graph.thickness = glm::max(graph.thickness, 0.0f);

		ImGui::Separator();
		if(ImGui::BeginTabBar(("Calc Mode##" + std::to_string(graphsHierarchySelected)).c_str()))
		{
			if(ImGui::BeginTabItem("GPU"))
			{
				graph.mode = CalcMode::GPU;
				ImGui::Text("f: ");
				ImGui::SameLine();
				ImGui::InputTextMultiline("##prop3", &graph.equation);

				if(ImGui::Button("submit"))
				{
					graph.data.clear();
					graphManager->AddVariables(graph);
					errorOut = graph.SetShader(graph.equation);
					if(errorOut != "") ImGui::OpenPopup("ShaderError");
				}
				if(ImGui::BeginPopupModal("ShaderError", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("No error? %s", errorOut.c_str());

					if(ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }

					ImGui::EndPopup();
				}

				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("CPU"))
			{
				graph.mode = CalcMode::CPU;
				ImGui::Text("y = ");
				ImGui::SameLine();
				ImGui::InputTextMultiline("##prop4", &graph.equation);

				if(ImGui::Button("submit"))
				{
					graphManager->SetVariables(graph);
					graphManager->UpdateVertices(graph, camera->leftBound, camera->rightBound);
				}

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		// equation

		// submit equation
		// std::cout << graph.equation << std::endl;

		ImGui::Separator();
		for(auto& [name, value] : graph.variables)
		{
			float drag = value;
			ImGui::Text("%s =", name.c_str());
			ImGui::SameLine();
			if(ImGui::DragFloat(("##" + name).c_str(), &drag, 0.01f))
			{
				value = drag;
				graphManager->UpdateVertices(graph, camera->leftBound, camera->rightBound);
			}
		}

		if(graph.variables.size() > 0) { ImGui::Separator(); }

		if(graph.mode == CalcMode::CPU)
		{
			ImGui::Text("Number of vertices %zu", graph.data.size());
			static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
			if(ImGui::TreeNode("Vertices"))
			{

				if(ImGui::BeginTable("table1", 2, flags))
				{
					ImGui::TableSetupColumn("X");
					ImGui::TableSetupColumn("Y");

					ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
					for(size_t dataIndex = 0; dataIndex < graph.data.size(); dataIndex += 2)
					{
						ImGui::TableNextRow();
						for(int column = 0; column < 2; column++)
						{
							ImGui::TableSetColumnIndex(column);
							if(ImGui::InputFloat(("##" + std::to_string(dataIndex + column)).c_str(),
							                     &graph.data[dataIndex + column]),
							   ImGuiWindowFlags_NoDecoration)
							{
								graph.mesh.Set(graph.data);
							}
						}
					}
					ImGui::EndTable();
				}
				ImGui::PopStyleColor();
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

void UI::Update()
{
	StartFrame();

	ImGui::SetNextWindowSize(ImVec2(window->GetWidth(), window->GetHeight()));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::ShowDemoWindow();

	MainWindow();
	ViewportWindow(this);
	InfoWindow(this);
	Settings(this);
	HierarchyWindow(this);
	PropertiesWindow(this);

	RenderUI();
}
