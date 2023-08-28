#include "QuoridorAgent.hpp"

QuoridorAgent::QuoridorAgent(bool is_player1)
{
    this->is_player1 = is_player1;
}

QuoridorAgent::~QuoridorAgent()
{
}

void QuoridorAgent::nextMove(QuoridorBoard game)
{
}

int8_t QuoridorAgent::isGameFinished(QuoridorBoard game, bool is_first_player)
{
    if (game.players[0].position_y == 8)
        if (is_first_player)
        {
            return 127;
        }
        else
        {
            return -127;
        }
    else if (game.players[1].position_y == 0)
        if (is_first_player)
        {
            return -127;
        }
        else
        {
            return 127;
        }
    else
        return 1;
}

int8_t QuoridorAgent::minimax(QuoridorBoard game, uint8_t alpha, uint8_t beta, uint8_t depth, bool is_first_player, bool is_max_node)
{
    int8_t gameFinished = this->isGameFinished(game = game, is_first_player = is_first_player);
    if (gameFinished == 127 || gameFinished == -127)
    {
        return gameFinished;
    }
    else if (depth == 0)
    {
        this->evaluationHeuristic(game = game, is_first_player = is_first_player);
    }
    else
    {
        return 0;
        //     if (is_max_node)
        //     {
        //         int8_t maxEval = -127;
        //         for (uint8_t i = 0; i < 81; i++)
        //         {
        //             if (game.isValidMove(i, is_first_player))
        //             {
        //                 QuoridorBoard new_game = game;
        //                 new_game.movePlayer(i, is_first_player);
        //                 int8_t eval = this->minimax(new_game, alpha, beta, depth - 1, is_first_player, false);
        //                 maxEval = max(maxEval, eval);
        //                 alpha = max(alpha, eval);
        //                 if (beta <= alpha)
        //                 {
        //                     break;
        //                 }
        //             }
        //         }
        //         return maxEval;
        //     }
        //     else
        //     {
        //         int8_t minEval = 127;
        //         for (uint8_t i = 0; i < 81; i++)
        //         {
        //             if (game.isValidMove(i, is_first_player))
        //             {
        //                 QuoridorBoard new_game = game;
        //                 new_game.movePlayer(i, is_first_player);
        //                 int8_t eval = this->minimax(new_game, alpha, beta, depth - 1, is_first_player, true);
        //                 minEval = min(minEval, eval);
        //                 beta = min(beta, eval);
        //                 if (beta <= alpha)
        //                 {
        //                     break;
        //                 }
        //             }
        //         }
        // return minEval;
    }
}

int8_t QuoridorAgent::evaluationHeuristic(QuoridorBoard game, bool is_first_player)
{
    Graph graph = Graph(81);
    Player player1 = game.players[0];
    Player player2 = game.players[1];

    int8_t position_node_player1 = graph.coords2NodeId(player1.position_x, player1.position_y);
    int8_t position_node_player2 = graph.coords2NodeId(player2.position_x, player2.position_y);
    int8_t min_distance_player1 = graph.getMinDistance(position_node_player1, true);
    int8_t min_distance_player2 = graph.getMinDistance(position_node_player2, false);
    int8_t num_barriers_player1 = player1.barriers_left;
    int8_t num_barriers_player2 = player2.barriers_left;

    int8_t value = min_distance_player2 - min_distance_player1 + 2 * (num_barriers_player1 - num_barriers_player2);

    if (is_first_player)
    {
        return value;
    }
    else
    {
        return -value;
    }
}