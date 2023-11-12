#include "graphManager.h"

GraphManager::GraphManager()
{
}

GraphManager::~GraphManager()
{
}

void GraphManager::AddGraph(Graph& graph)
{
	graphs.push_back(graph);
	UpdateNames();
}

void GraphManager::UpdateNames()
{
	graphNames.clear();

	for(const auto& graph : graphs) { graphNames.push_back((graph.name.c_str())); }
}

bool GraphManager::LoadGraphCVT(const char* path)
{
	std::ifstream file(path);
	if(!file.is_open())
	{
		std::cerr << "Failed to open the file." << std::endl;
		return 0;
	}

	std::vector<float> data; // Vector to store x, y pairs

	float x, y;
	while(file >> x >> y)
	{
		data.push_back(x);
		data.push_back(y);
	}

	file.close();

	Graph graph("a");
	// TODO: add graph.SetData()

	AddGraph(graph);

	return 1;
}
