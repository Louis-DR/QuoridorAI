#include "Graph.hpp"

Graph::Graph(Array2D<Array2D<bool,9>,9> adjacencyTables) {
    this->adjacencyTables = adjacencyTables;
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
    for (size_t i = std::max(0,node_coords.first - 2); i <= std::min(8,node_coords.first + 3); i++){
        for (size_t j = std::max(0, node_coords.second -2); j <= std::min(8, node_coords.second + 3); j++){
            if (this->adjacencyTables[node_coords.first][node_coords.second][i][j]){
                //std::cout << "Neighbor ("<< +i << ", "<< +j <<")" << endl;
                neighbors.push_back(std::pair<uint8_t, uint8_t>(i,j));
            }
        }
    }
    return neighbors;
}

uint8_t Graph::coords2NodeId(uint8_t x, uint8_t y) {
    return y * this->final_line_size + x;
}

bool Graph::isNodeOnFinishLine(std::pair<uint8_t, uint8_t> node, bool is_first_player)
{   
    //std::cout << "is_first_player "<< is_first_player << endl;
    if (is_first_player) {
        //std::cout << "node.second "<< +node.second << endl;
        return node.second == 8;
    else {
        return node.second == 0;
    }
}

uint8_t Graph::getMinDistance(std::pair<uint8_t, uint8_t> startNode, bool is_first_player, std::set<std::pair<uint8_t, uint8_t>> stopping_nodes)
{   

    Array2D<bool,9> visited = Array2D<bool,9>();
    Array2D<uint8_t,9> distance = Array2D<uint8_t,9>();
    Array2D<uint8_t,9> parent = Array2D<uint8_t,9>();
    distance[startNode.first][startNode.second] = 0;
    if (this->isNodeOnFinishLine(startNode, is_first_player)){ //|| stopping_nodes.find(startNode) != stopping_nodes.end())
        return 0;
    }

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
                bool isFinishNode = this->isNodeOnFinishLine(neighbor, is_first_player);
                if (isFinishNode){// || stopping_nodes.find(neighbor) != stopping_nodes.end())
                    uint8_t final_distance = distance[node.first][node.second] + 1;
                    return final_distance;
                }
                if (visited[neighbor.first][neighbor.second]){
                    continue;
                }
                distance[neighbor.first][neighbor.second] = distance[node.first][node.second] + 1;
                visited[neighbor.first][neighbor.second] = true;
                newFrontier.insert(neighbor);
                parent[neighbor.first][neighbor.second] = this->coords2NodeId(node.first, node.second);
            }

        }
        frontier = newFrontier;
        newFrontier = set<std::pair<uint8_t, uint8_t>>();
        if (frontier.empty()){
            break;
        }
    }
    return 255;
}
