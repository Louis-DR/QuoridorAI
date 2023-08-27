#include "CorridorAgent.hpp"

CorridorAgent::CorridorAgent(bool is_player1)
{
    this->is_player1 = is_player1;
}

CorridorAgent::~CorridorAgent()
{
}

void CorridorAgent::nextMove(QuoridorBoard game)
{
}

int CorridorAgent::minimax(QuoridorBoard game, int depth, bool is_first_player)
{
    return 0;
}

uint8_t CorridorAgent::evaluationHeuristic(QuoridorBoard game)
{
    graph = Graph(81);
    Player player1 = game.players[0];
    Player player2 = game.players[1];

    uint8_t position_node_player1 = graph.coords2NodeId(player1.position_x, player1.position_y);
    uint8_t position_node_player1 = graph.coords2NodeId(player2.position_x, player2.position_y);
    uint8_t min_distance_player1 = graph.getMinDistance(position_node_player1, true);
    uint8_t min_distance_player2 = graph.getMinDistance(position_node_player1, false);
    uint8_t num_barriers_player1 = player1.barriers_left;
    uint8_t num_barriers_player2 = player2.barriers_left;

    return min_distance_player2 - min_distance_player1 + 2 * (num_barriers_player1 - num_barriers_player2);
}