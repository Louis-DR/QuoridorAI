#pragma once

using namespace std;
#include "QuoridorBoard.hpp"
#include "Graph.hpp"

class CorridorAgent
{
    bool is_player1;

public:
    CorridorAgent(bool is_player1);
    ~CorridorAgent();
    void nextMove(QuoridorBoard game);
    uint8_t minimax(QuoridorBoard game, int depth, bool isMaximizingPlayer);
    uint8_t evaluationHeuristic(QuoridorBoard game);
};
