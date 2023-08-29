#pragma once

#include "QuoridorBoard.hpp"
#include "Graph.hpp"
#include <iostream>

using namespace std;

class QuoridorAgent
{
    bool is_player1;

public:
    QuoridorAgent(bool is_player1);
    ~QuoridorAgent();
    void    nextMove(QuoridorBoard game);
    int8_t  isGameFinished(QuoridorBoard game, bool is_first_player);
    int8_t  minimax(QuoridorBoard game, uint8_t alpha, uint8_t beta, uint8_t depth, bool is_first_player, bool is_max_node);
    uint8_t getMinDistancePlayer(QuoridorBoard game, bool is_first_player);
    int8_t  evaluationHeuristic(QuoridorBoard game, bool is_first_player);
};
