#pragma once

#include <fstream>
#include <sstream>
#include <vector>

#include "graph.h"
#include "unordered_map"
#include <unordered_map>
#include <iostream>

class GraphManager
{

  public:
	GraphManager();
	~GraphManager();

	void AddGraph(Graph& graph);
	void AddGrid(Graph& graph);

	void UpdateNames();

	bool LoadGraphCVT(const char* path);

	std::vector<Graph> graphs;
	std::vector<const char*> graphNames;
};
