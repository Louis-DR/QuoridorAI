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

using namespace std;

class Graph
{
private:
    uint8_t vertexCount;
    std::vector<bool> visited;
    std::vector<std::vector<bool>> adjMatrix;
    std::vector<uint8_t> distance;
    std::vector<uint8_t> parent;
    uint8_t final_line_size;

public:
    Graph(uint8_t vertexCount);
    ~Graph();
    uint8_t getMinDistance(uint8_t startNode, bool is_first_player);
    void addEdge(uint8_t i, uint8_t j);
    void removeEdge(uint8_t i, uint8_t j);
    std::vector<uint8_t> getNeighbors(uint8_t node);
    uint8_t coords2NodeId(uint8_t x, uint8_t y);
    bool isNodeOnFinishLine(uint8_t node, bool is_first_player);
};