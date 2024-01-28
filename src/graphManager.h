#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "graph.h"
#include "tinyexpr/tinyexpr.h"

class GraphManager
{
  public:
	GraphManager();
	~GraphManager();

	void AddGraph(Graph* graph);
	void AddGrid(Graph& graph);

	void UpdateNames();

	bool UpdateVertices(Graph& graph, float leftBound, float rightBound);
	bool UpdateAllGraphVertices(float leftBound, float rightBound, bool skipCheck = false);
	void SetVariables(Graph& graph);

	bool LoadGraphCVT(const char* path);

	// if camera out of boundOffset + boundLeft/Right, regenerate vertices
	float boundOffset = 5.0f;
	float increment = 0.1f;

	te_parser parser;

	// dynamic float, goes from left most side of camera view to most right side
	double x = 0.0f;
	double y = 1.0f;

	std::vector<Graph*> newGraphs;
	std::vector<Graph*> graphs;
	std::vector<const char*> graphNames;
};
