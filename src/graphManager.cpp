#include "graphManager.h"

static std::set<char> GetExprVariables(std::string input)
{
	input.push_back(' ');
	input.insert(0, 1, ' ');
	std::set<char> out;

	for(size_t i = 1; i < input.size(); i++)
	{
		if(std::isalpha(input[i]))
		{
			if(std::isalpha(input[i - 1]) || std::isalpha(input[i + 1])) continue;
			out.insert(input[i]);
		}
	}
	return out;
}

GraphManager::GraphManager()
{
	graphs.reserve(3);
}

GraphManager::~GraphManager()
{
	// delete graphs
	for(auto& graph : graphs) { delete graph; }
}

void GraphManager::DeleteGraph(unsigned int index)
{
	graphs.erase(graphs.begin() + index);
	UpdateNames();
}

void GraphManager::AddGraph(Graph* graph)
{
	graphs.push_back(graph);

	UpdateNames();
}

void GraphManager::UpdateNames()
{
	graphNames.clear();

	for(const auto& graph : graphs) { graphNames.push_back((graph->name.c_str())); }
}

void GraphManager::SetVariables(Graph& graph)
{

	std::set<te_variable> varSet { { "x", &x } };

	auto varList = GetExprVariables(graph.equation);
	for(auto var : varList)
	{
		if(var == 'x') continue;
		graph.variables[std::string(1, var)];
	}

	for(auto& pair : graph.variables) { varSet.insert({ pair.first, &pair.second }); }

	graph.parser.set_variables_and_functions(varSet);
}

bool GraphManager::UpdateAllGraphVertices(float leftBound, float rightBound, bool skipCheck)
{

	// increment = (glm::abs(leftBound - boundOffset) + glm::abs(rightBound + boundOffset)) / (2 * 1000);
	newGraphs.clear();
	newGraphs.reserve(graphs.size());

	for(auto* graph : graphs)
	// for(std::vector<Graph*>::iterator it = graphs.begin(); it != graphs.end(); ++it)
	{
		if((graph->leftMostX < leftBound && graph->rightMostX > rightBound) && !skipCheck) { continue; };
		newGraphs.push_back(graph);
	}
	if(newGraphs.size() == 0) { return 1; }

	// clear all graphs data
	for(auto& graph : newGraphs)
	{
		graph->data.clear();

		graph->leftMostX = leftBound - boundOffset;
		graph->rightMostX = rightBound + boundOffset;

		graph->data.reserve((glm::abs(graph->leftMostX) + glm::abs(graph->rightMostX)) / increment * 2);
	}

	for(x = leftBound - boundOffset; x <= rightBound + boundOffset; x += increment)
	{
		for(auto& graph : newGraphs)
		{
			graph->data.push_back(x);
			graph->data.push_back(graph->parser.evaluate(graph->equation));
		}
	}

	// set all graphs data
	for(auto& graph : newGraphs) { graph->SetData(graph->data); }

	return 1;
}
bool GraphManager::UpdateVertices(Graph& graph, float leftBound, float rightBound)
{
	graph.leftMostX = leftBound - boundOffset;
	graph.rightMostX = rightBound + boundOffset;

	std::vector<float> vertices;
	vertices.reserve((glm::abs(graph.leftMostX) + glm::abs(graph.rightMostX)) / graph.increment * 2);

	// std::cout << graph.name << " = " << graph.equation << std::endl;
	//   std::cout << leftBound << " ; " << rightBound << std::endl;

	for(x = leftBound - boundOffset; x <= rightBound + boundOffset; x += increment)
	{
		vertices.push_back(x);
		vertices.push_back(graph.parser.evaluate(graph.equation));
	}

	graph.SetData(vertices);

	return 1;
}
