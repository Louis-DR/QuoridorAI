#include <iostream>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
#include <array>
#include <stdint.h>
#include <math.h>
#include <limits.h>
#include <set>
#include <QuoridorBoard.hpp>
#include <utility>

using namespace std;

class Graph
{
private:
    uint8_t vertexCount;
    Array2D<bool,9> visited;
    Array2D<Array2D<bool,9>,9> adjacencyTables;
    Array2D<uint8_t,9> distance;
    Array2D<uint8_t,9> parent;
    uint8_t final_line_size;

public:
    Graph(Array2D<Array2D<bool,9>,9> adjacencyTables);
    ~Graph();
    uint8_t getMinDistance(std::pair<uint8_t, uint8_t> startNode, bool is_first_player, std::set<std::pair<uint8_t, uint8_t>> stopping_nodes = set<std::pair<uint8_t, uint8_t>>());
    void addEdge(uint8_t i, uint8_t j);
    void removeEdge(uint8_t i, uint8_t j);
    std::vector<std::pair<uint8_t, uint8_t>>  getNeighbors(std::pair<uint8_t, uint8_t>  node_coords);
    uint8_t coords2NodeId(uint8_t x, uint8_t y);
    bool isNodeOnFinishLine(std::pair<uint8_t, uint8_t> node, bool is_first_player);
    std::pair<uint8_t, uint8_t> nodeId2Coords(uint8_t node);
};