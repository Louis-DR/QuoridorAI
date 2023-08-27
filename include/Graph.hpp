#include <iostream>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
#include <array>

using namespace std;

class Graph
{
private:
    int vertexCount;
    std::vector<bool> visited;
    std::vector<std::vector<bool>> adjMatrix;
    std::vector<int> distance;
    std::vector<int> parent;
    int final_line_size;

public:
    Graph(int vertexCount);
    ~Graph();
    int getMinDistance(int startNode, bool is_first_player);
    void addEdge(int i, int j);
    void removeEdge(int i, int j);
    std::vector<int> getNeighbors(int node);
    int coords2NodeId(int x, int y);
    bool isNodeOnFinishLine(int node, bool is_first_player);
};