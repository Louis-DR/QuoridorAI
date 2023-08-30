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
            return 81;
        }
        else
        {
            return -81;
        }
    else if (game.players[1].position_y == 0)
        if (is_first_player)
        {
            return -81;
        }
        else
        {
            return 81;
        }
    else
        return 1;
}

Move QuoridorAgent::get_bestMove(QuoridorBoard game, vector<Move> moves, bool is_first_player, uint8_t depth){
    uint8_t bestMoveIndex = 0;
    int8_t alpha = -100;
    int8_t beta = 100;
    for (uint8_t i = 0; i < moves.size(); i++)
    {
        cout << "EVALUATING depth=" << +depth << " move_i=" << i+1 << "/" << +moves.size() << " start" << endl;
        game.debug_printMove(moves[i]);
        game.doMove(moves[i]);
        int8_t value = this->minimax(game, alpha, beta, depth - 1, is_first_player, false, 1);
        game.undoMove(moves[i]);
        if (value > alpha)
        {
            bestMoveIndex = i;
            alpha = value;
        }
    }
    cout << "BEST MOVE VALUE = " << +alpha << endl;
    return moves[bestMoveIndex];
}

int8_t QuoridorAgent::minimax(QuoridorBoard game, int8_t alpha, int8_t beta, uint8_t depth, bool is_first_player, bool is_max_node, uint8_t indent){
    if (depth == 0)
    {
        int8_t moveValue = evaluationHeuristic(game, is_first_player);
        return moveValue;
    }
    else {
        if (is_max_node) {
            int8_t maxEval = -100;
            std::vector<Move> moves = game.get_legalMoves(is_first_player);
            for (uint8_t i = 0; i < moves.size(); i++)
            {
                for (size_t i = 0; i < indent; ++i) cout << "  ";
                cout << "EVALUATING depth=" << +depth << " move_i=" << i+1 << "/" << +moves.size() << " max" << endl;
                game.debug_printMove(moves[i], indent);
                game.doMove(moves[i]);
                int8_t value = this->minimax(game, alpha, beta, depth - 1, is_first_player, false, indent+1);
                for (size_t i = 0; i < indent; ++i) cout << "  ";
                cout << "value=" << +value << endl;
                game.undoMove(moves[i]);
                maxEval = max(maxEval, value);
                if (maxEval >= beta)
                {
                    return maxEval;
                }
                alpha = max(alpha, value);
            }
            return maxEval;
        }
        else {
                int8_t minEval = 100;
                std::vector<Move> moves = game.get_legalMoves(!is_first_player);
                for (uint8_t i = 0; i < moves.size(); i++)
                {
                    for (size_t i = 0; i < indent; ++i) cout << "  ";
                    cout << "EVALUATING depth=" << +depth << " move_i=" << i+1 << "/" << +moves.size() << " min" << endl;
                    game.debug_printMove(moves[i]);
                    game.doMove(moves[i]);
                    int8_t value = this->minimax(game, alpha, beta, depth - 1, is_first_player, true, indent+1);
                    game.undoMove(moves[i]);
                    for (size_t i = 0; i < indent; ++i) cout << "  ";
                    cout << "value=" << +value << endl;
                    minEval = min(minEval, value);
                    if (minEval <= alpha)
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
    int8_t gameFinished = this->isGameFinished(game, is_first_player);
    if (gameFinished == 81 || gameFinished == -81)
        return gameFinished;

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
    // int8_t value = min_distance_player2 - min_distance_player1;

    if (is_first_player)
    {
        cout << "  min_distance_player1=" << +min_distance_player1 << endl;
        return value;
    }
    else
    {
        cout << "  min_distance_player2=" << +min_distance_player2 << endl;
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