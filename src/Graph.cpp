#include "Graph.hpp"

Graph::Graph(uint8_t vertexCount)
{
    this->vertexCount = vertexCount;
    this->visited = std::vector<bool>(vertexCount, false);
    this->distance = std::vector<uint8_t>(vertexCount, 255);
    this->parent = std::vector<uint8_t>(vertexCount, -1);
    this->final_line_size = static_cast<uint8_t>(sqrt(vertexCount));
    this->adjMatrix = std::vector<std::vector<bool>>(vertexCount, std::vector<bool>(vertexCount, false));
}
Graph::~Graph()
{
}

void Graph::addEdge(uint8_t i, uint8_t j)
{
    if (i >= 0 && i < vertexCount && j >= 0 && j < vertexCount)
    {
        adjMatrix[i][j] = true;
        adjMatrix[j][i] = true;
    }
}
void Graph::removeEdge(uint8_t i, uint8_t j)
{
    if (i >= 0 && i < vertexCount && j >= 0 && j < vertexCount)
    {
        adjMatrix[i][j] = false;
        adjMatrix[j][i] = false;
    }
}

std::vector<uint8_t> Graph::getNeighbors(uint8_t node)
{
    std::vector<uint8_t> neighbors;
    for (size_t i = 0; i < vertexCount; i++)
    {
        if (adjMatrix[node][i])
        {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}
uint8_t Graph::coords2NodeId(uint8_t x, uint8_t y)
{
    return y * this->final_line_size + x;
}

bool Graph::isNodeOnFinishLine(uint8_t node, bool is_first_player)
{
    if (is_first_player)
    {
        if (node > this->vertexCount - this->final_line_size - 1)
            return true;
        else
            return false;
    }
    else
    {
        if (node < this->final_line_size)
            return true;
        else
            return false;
    }
}

uint8_t Graph::getMinDistance(uint8_t startNode, bool is_first_player)
{

    this->distance[startNode] = 0;

    uint8_t minDistanceNode, i;
    std::set<uint8_t> frontier = set<uint8_t>();
    frontier.insert(startNode);
    std::set<uint8_t>::iterator itr;
    while (true)
    {
        std::set<uint8_t> newFrontier = set<uint8_t>();
        for (itr = frontier.begin(); itr != frontier.end(); itr++)
        {
            uint8_t node = *itr;
            std::vector<uint8_t>
                temp = getNeighbors(node);
            for (size_t j = 0; j < temp.size(); j++)
            {
                uint8_t neighbor = temp[j];
                if (this->isNodeOnFinishLine(neighbor, is_first_player))
                    return this->distance[node] + 1;
                if (this->visited[neighbor])
                    continue;
                this->distance[neighbor] = this->distance[node] + 1;
                this->visited[neighbor] = true;
                newFrontier.insert(neighbor);
                this->parent[neighbor] = node;
            }
            frontier = newFrontier;
            newFrontier = set<uint8_t>();
        }
    }
}
