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
        return 0;
}

Move QuoridorAgent::get_bestMove(QuoridorBoard game, vector<Move> moves, bool is_first_player, uint8_t depth){
    auto chrono_start = chrono::high_resolution_clock::now();
    uint32_t numberPositionSearched = 0;
    uint8_t bestMoveIndex = 0;
    int8_t alpha = -100;
    int8_t beta = 100;
    for (uint8_t i = 0; i < moves.size(); i++)
    {
        game.doMove(moves[i]);
        numberPositionSearched++;
        int8_t value = this->minimax(game, alpha, beta, depth - 1, is_first_player, false, numberPositionSearched);
        game.undoMove(moves[i]);
        if (value > alpha)
        {
            bestMoveIndex = i;
            alpha = value;
        }
    }
    auto chrono_stop = chrono::high_resolution_clock::now();
    chrono::duration<float> chrono_duration = chrono_stop - chrono_start;
    float searchSpeed = float(numberPositionSearched)/chrono_duration.count();
    cout << "numberPositionSearched=" << +numberPositionSearched << endl;
    cout << "searchDuration=" << chrono_duration.count() << "s" << endl;
    cout << "searchSpeed=" << searchSpeed << "position/s" << endl;
    searchSpeeds.push_back(searchSpeed);
    return moves[bestMoveIndex];
}

int8_t QuoridorAgent::minimax(QuoridorBoard game, int8_t alpha, int8_t beta, uint8_t depth, bool is_first_player, bool is_max_node, uint32_t& numberPositionSearched){
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
                game.doMove(moves[i]);
                numberPositionSearched++;
                int8_t value = this->minimax(game, alpha, beta, depth - 1, is_first_player, false, numberPositionSearched);
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
                game.doMove(moves[i]);
                numberPositionSearched++;
                int8_t value = this->minimax(game, alpha, beta, depth - 1, is_first_player, true, numberPositionSearched);
                game.undoMove(moves[i]);
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

float QuoridorAgent::get_averageSearchSpeed() {
    if (searchSpeeds.empty()) return 0;
    return reduce(searchSpeeds.begin(), searchSpeeds.end()) / searchSpeeds.size();
}