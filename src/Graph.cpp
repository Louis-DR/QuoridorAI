#include "Graph.hpp"
#include <math.h>
Graph::Graph(int vertexCount)
{
    this->vertexCount = vertexCount;
    this->visited = std::vector<bool>(vertexCount, false);
    this->distance = std::vector<int>(vertexCount, INT_MAX);
    this->parent = std::vector<int>(vertexCount, -1);
    this->final_line_size = static_cast<int>(sqrt(vertexCount));
    this->adjMatrix = std::vector<std::vector<bool>>(vertexCount, std::vector<bool>(vertexCount, false));
}
Graph::~Graph()
{
}

void Graph::addEdge(int i, int j)
{
    if (i >= 0 && i < vertexCount && j >= 0 && j < vertexCount)
    {
        adjMatrix[i][j] = true;
        adjMatrix[j][i] = true;
    }
}
void Graph::removeEdge(int i, int j)
{
    if (i >= 0 && i < vertexCount && j >= 0 && j < vertexCount)
    {
        adjMatrix[i][j] = false;
        adjMatrix[j][i] = false;
    }
}
int Graph::findMinDistanceNode()
{
    int minDistant = INT_MAX;
    int minDistantNode;

    for (int i = 0; i < vertexCount; i++)
    {
        if (minDistant > this->distance[i] && this->key[i] == 0)
        {
            minDistantNode = i;
            minDistant = this->distance[i];
        }
    }
    return minDistantNode;
}

std::vector<int> Graph::getNeighbors(int node)
{
    std::vector<int> neighbors;
    for (int i = 0; i < vertexCount; i++)
    {
        if (adjMatrix[node][i])
        {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}
int Graph::coords2NodeId(int x, int y)
{
    return y * this->final_line_size + x;
}

bool Graph::isNodeOnFinishLine(int node, bool is_first_player) :
{
    if (is_first_player)
    {
        if node
            > this->vertexCount - this->final_line_size - 1 return true;
        else
            return false;
    }
    else
    {
        if node
            < this->final_line_size return true;
        else
            return false;
    }
}

void Graph::getMinDistance(int startNode, bool is_first_player)
{

    this->distance[startNode] = 0;

    int minDistanceNode, i;
    std::set<int> frontier = set<int>();
    frontier.insert(startNode);
    while (true)
    {
        std::set<int> newFrontier = set<int>();
        for (int i = 0, i < frontier.size(); i++)
        {
            int node = frontier[i];
            std::vector<int> temp = getNeighbors(node);
            for (int j = 0; j < temp.size(); j++)
            {
                int neighbor = neighbors[j];
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
            newFrontier = set<int>();
        }
    }
}
