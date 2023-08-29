#include "Graph.hpp"

Graph::Graph(Array2D<Array2D<bool,9>,9> adjacencyTables) {
    this->adjacencyTables = adjacencyTables;
    // this->visited = std::vector<bool>(this->adjacencyTables, false);
    // this->distance = std::vector<int8_t>(vertexCount, 127);
    // this->parent = std::vector<uint8_t>(vertexCount, -1);
    // this->final_line_size = static_cast<uint8_t>(sqrt(vertexCount));
    this->visited = Array2D<bool,9>();
    this->distance = Array2D<uint8_t,9>();
    this->parent = Array2D<uint8_t,9>();
    this->final_line_size = 9;
}

Graph::~Graph() {}

void Graph::addEdge(uint8_t i, uint8_t j) {
    std::pair<uint8_t, uint8_t> coords_i = this->nodeId2Coords(i);
    std::pair<uint8_t, uint8_t> coords_j = this->nodeId2Coords(j);
    this-> adjacencyTables[coords_i.first][coords_i.second][coords_j.first][coords_j.second] = true;
}

void Graph::removeEdge(uint8_t i, uint8_t j) {
    std::pair<uint8_t, uint8_t> coords_i = this->nodeId2Coords(i);
    std::pair<uint8_t, uint8_t> coords_j = this->nodeId2Coords(j);
    this->adjacencyTables[coords_i.first][coords_i.second][coords_j.first][coords_j.second] = false;
}

std::vector<std::pair<uint8_t, uint8_t>> Graph::getNeighbors(std::pair<uint8_t, uint8_t> node_coords) {
    std::vector<std::pair<uint8_t, uint8_t>>  neighbors;
    for (size_t i = std::max(0,node_coords.first - 2); i < std::min(8,node_coords.first + 2); i++){
        for (size_t j = std::max(0, node_coords.second -2); j < std::min(8, node_coords.second + 2); j++){
            if (this->adjacencyTables[node_coords.first][node_coords.second][i][j]){
                neighbors.push_back(std::pair<uint8_t, uint8_t>(i,j));
            }
        }
    }
    return neighbors;
}

uint8_t Graph::coords2NodeId(uint8_t x, uint8_t y) {
    return y * this->final_line_size + x;
}

std::pair<uint8_t, uint8_t> Graph::nodeId2Coords(uint8_t node) {
    return std::pair<uint8_t, uint8_t>(node % this->final_line_size, node / this->final_line_size);
}

bool Graph::isNodeOnFinishLine(std::pair<uint8_t, uint8_t> node, bool is_first_player) {
    if (is_first_player)
        return node.second == 8;
    else
        return node.second == 0;
}

uint8_t Graph::getMinDistance(std::pair<uint8_t, uint8_t> startNode, bool is_first_player, std::set<std::pair<uint8_t, uint8_t>> stopping_nodes) {
    this->distance[startNode.first][startNode.second] = 0;

    uint8_t minDistanceNode, i;
    std::set<std::pair<uint8_t, uint8_t>> frontier = set<std::pair<uint8_t, uint8_t>>();
    frontier.insert(startNode);
    std::set<std::pair<uint8_t, uint8_t>>::iterator itr;
    while (true)
    {
        std::set<std::pair<uint8_t, uint8_t>> newFrontier = set<std::pair<uint8_t, uint8_t>>();
        for (itr = frontier.begin(); itr != frontier.end(); itr++)
        {
            std::pair<uint8_t, uint8_t> node = *itr;
            std::vector<std::pair<uint8_t, uint8_t>> temp = getNeighbors(node);
            for (size_t j = 0; j < temp.size(); j++)
            {
                std::pair<uint8_t, uint8_t> neighbor = temp[j];
                if (this->isNodeOnFinishLine(neighbor, is_first_player) || stopping_nodes.find(neighbor) != stopping_nodes.end())
                    return this->distance[node.first][node.second] + 1;
                if (this->visited[neighbor.first][neighbor.second])
                    continue;
                this->distance[neighbor.first][neighbor.second] = this->distance[node.first][node.second] + 1;
                this->visited[neighbor.first][neighbor.second] = true;
                newFrontier.insert(neighbor);
                this->parent[neighbor.first][neighbor.second] = this->coords2NodeId(node.first, node.second);
            }
            frontier = newFrontier;
            newFrontier = set<std::pair<uint8_t, uint8_t>>();
        }
        if (frontier.empty())
            break;
    }
    return 255;
}
