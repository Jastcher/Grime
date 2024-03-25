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
	void DeleteGraph(unsigned int index);

	void UpdateNames();

	void SetVariables(Graph& graph);
	void AddVariables(Graph& graph);
	bool UpdateVertices(Graph& graph, float leftBound, float rightBound);
	bool UpdateAllGraphVertices(float leftBound, float rightBound, bool skipCheck = false);

	// if camera out of boundOffset + boundLeft/Right, regenerate vertices
	float boundOffset = 5.0f;

	// x += increment
	float increment = 0.1f;

	// dynamic float, goes from left most side of camera view to most right side
	double x = 0.0f;

	std::vector<Graph*> newGraphs;
	std::vector<Graph*> graphs;
	std::vector<const char*> graphNames;
};
