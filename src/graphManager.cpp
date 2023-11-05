#include "graphManager.h"


GraphManager::GraphManager()
{

	Graph gridX("Grid X", glm::vec3(0.35f), {0.0f, 1.0f, 0.0f, -1.0f}, 2.0f);
    AddGrid(gridX);

	Graph gridY("Grid Y", glm::vec3(0.35f), {1.0f, 0.0f, -1.0f, 0.0f}, 2.0f);
    AddGrid(gridY);

	Graph mainAxisX("Axis X", glm::vec3(0.3f, 1.0f, 0.3f), {-1.0f, 0.0f, 1.0f, 0.0f}, 4.0f);
    AddGrid(mainAxisX);

	Graph mainAxisY("Axis Y", glm::vec3(1.0f, 0.3f, 0.3f), {0.0f, 1.0f, 0.0f, -1.0f}, 4.0f);
    AddGrid(mainAxisY);
}

GraphManager::~GraphManager(){}

void GraphManager::AddGraph(Graph& graph)
{
    graphs.push_back(graph);

    char* str;
    graph.name.copy(str, graph.name.size());
    graphNames.push_back(str);
}
void GraphManager::AddGrid(Graph& graph)
{
    grid.push_back(graph);
    //std::cout << "Added grid with name: " << graph.name << std::endl;
    char* str;
    graph.name.copy(str, graph.name.size());
    gridNames.push_back(str);
}

bool GraphManager::LoadGraphCVT(const char* path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 0;
    }

    std::vector<float> data; // Vector to store x, y pairs

    float x, y;
    while (file >> x >> y) {
        data.push_back(x);
        data.push_back(y);
    }

    file.close();

    Graph graph("a", glm::vec3(1.0f), data, 3.0f);
    AddGraph(graph);

    return 1;

}
