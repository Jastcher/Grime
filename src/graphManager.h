#pragma once

#include <fstream>
#include <sstream>
#include <vector>

#include "graph.h"
#include "unordered_map"
#include <unordered_map>
#include <iostream>

class GraphManager {

public:
    GraphManager();
    ~GraphManager();

    void AddGraph(Graph& graph);
    void AddGrid(Graph& graph);

    bool LoadGraphCVT(const char* path);

    std::vector<const char*> gridNames;
    std::vector<const char*> graphNames;
    std::vector<Graph> graphs;
    std::vector<Graph> grid;
};
