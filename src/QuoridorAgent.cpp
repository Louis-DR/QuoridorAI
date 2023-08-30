#include "QuoridorAgent.hpp"

QuoridorAgent::QuoridorAgent()
{
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

Move QuoridorAgent::get_bestMove(QuoridorBoard game, vector<Move> moves, bool is_first_player, uint8_t depth){
    uint8_t bestMoveIndex = -1;
    int8_t alpha = -127;
    int8_t beta = 127;
    for (uint8_t i = 0; i < moves.size(); i++)
    {
        game.doMove(moves[i]);
        int8_t value = this->minimax(game, alpha, beta, depth - 1, is_first_player, false);
        game.undoMove(moves[i]);
        if (value > alpha)
        {
            bestMoveIndex = i;
            alpha = value;
        }
    }
    return moves[bestMoveIndex];
}

int8_t QuoridorAgent::minimax(QuoridorBoard game, int8_t alpha, int8_t beta, uint8_t depth, bool is_first_player, bool is_max_node){
    int8_t gameFinished = this->isGameFinished(game = game, is_first_player = is_first_player);
    if (gameFinished == 127 || gameFinished == -127)
    {
        return gameFinished;
    }
    else if (depth == 0)
    {
        return this->evaluationHeuristic(game = game, is_first_player = is_first_player);
    }
    else {
        if (is_max_node) {
            int8_t maxEval = -127;
            std::vector<Move> moves = game.get_legalMoves(is_first_player);
            for (uint8_t i = 0; i < moves.size(); i++)
            {
                game.doMove(moves[i]);
                int8_t value = this->minimax(game, alpha, beta, depth - 1, is_first_player, false);
                game.undoMove(moves[i]);
                maxEval = max(maxEval, value);
                if (beta <= maxEval)
                {
                    return maxEval;
                }
                alpha = max(alpha, value);
            }
            return maxEval;
        }
        else {
                int8_t minEval = 127;
                std::vector<Move> moves = game.get_legalMoves(is_first_player);
                for (uint8_t i = 0; i < moves.size(); i++)
                {
                    game.doMove(moves[i]);
                    int8_t value = this->minimax(game, alpha, beta, depth - 1, is_first_player, true);
                    game.undoMove(moves[i]);
                    minEval = min(minEval, value);
                    if (beta <= minEval)
                    {
                        return minEval;
                    }
                    beta = min(beta, value);
                }

            return minEval;
        }
    }
}

int8_t QuoridorAgent::evaluationHeuristic(QuoridorBoard game, bool is_first_player)
{
    Graph graph = Graph(game.get_adjacencyTables());
    Player player1 = game.players[0];
    Player player2 = game.players[1];

    int8_t position_node_player1 = graph.coords2NodeId(player1.position_x, player1.position_y);
    int8_t position_node_player2 = graph.coords2NodeId(player2.position_x, player2.position_y);
    int8_t min_distance_player1 = graph.getMinDistance(std::pair<uint8_t, uint8_t>(player1.position_x, player1.position_y), true);
    int8_t min_distance_player2 = graph.getMinDistance(std::pair<uint8_t, uint8_t>(player2.position_x, player2.position_y), false);
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

uint8_t QuoridorAgent::getMinDistancePlayer(QuoridorBoard game, bool is_first_player){
    Graph graph = Graph(game.get_adjacencyTables());
    Player player1 = game.players[0];
    Player player2 = game.players[1];
    if (is_first_player){
        return graph.getMinDistance(std::pair<uint8_t, uint8_t>(player1.position_x, player1.position_y), true);
    }
    else{
        return graph.getMinDistance(std::pair<uint8_t, uint8_t>(player2.position_x, player2.position_y), false);
    }
}