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
#include <utility>

#include "QuoridorBoard.hpp"

using namespace std;

class Graph
{
private:
    Array2D<Array2D<bool,9>,9> adjacencyTables;
    uint8_t final_line_size;

public:
    Graph(Array2D<Array2D<bool,9>,9> adjacencyTables);
    ~Graph();
    void                           addEdge(uint8_t i, uint8_t j);
    void                           removeEdge(uint8_t i, uint8_t j);
    uint8_t                        coords2NodeId(uint8_t x, uint8_t y);
    pair<uint8_t, uint8_t>         nodeId2Coords(uint8_t node);
    vector<pair<uint8_t, uint8_t>> getNeighbors(pair<uint8_t, uint8_t>  node_coords);
    bool                           isNodeOnFinishLine(pair<uint8_t, uint8_t> node, bool is_first_player);
    uint8_t                        getMinDistance(pair<uint8_t, uint8_t> startNode, bool is_first_player, set<pair<uint8_t, uint8_t> > stopping_nodes = set<pair<uint8_t, uint8_t> >());
};