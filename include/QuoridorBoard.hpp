#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <random>
#include <algorithm>
#include <functional>
#include <chrono>
#include <thread>
#include <unordered_set>

using namespace std;

template <class T, size_t N> using Array2D = array<array<T,N>,N>;

uint64_t array8x8bool_to_uint64(Array2D<bool,8> array8x8bool);

struct BarrierGrid {
  Array2D<bool,8> horizontal;
  Array2D<bool,8> vertical;
};

struct Player {
  uint8_t position_x;
  uint8_t position_y;
  uint8_t barriers_left;
};

struct BoardConfig {
  bool interactive_auto_print = false;
  bool print_dot_grid = false;
  uint8_t ai_minimax_depth = 1;
};

struct PrintArgs {
  bool    adjacent_table_enable = false;
  uint8_t adjacent_table_x      = 0;
  uint8_t adjacent_table_y      = 0;
  bool    legal_barrier_enable  = false;
};

// Move that can be undone
struct Move {
  bool    isBarrierPlacement; // Else it is a player move
  bool    player_isWhite; // Else is black player
  uint8_t player_originalPosition_x;
  uint8_t player_originalPosition_y;
  uint8_t player_movePosition_x;
  uint8_t player_movePosition_y;
  bool    barrier_isHorizontal; // Else is vertical
  uint8_t barrier_position_x;
  uint8_t barrier_position_y;
  bool operator==(const Move& other) const;
};

template <> struct hash<Move> {
  size_t operator()(const Move& move) const {
    uint32_t movePosition = ( uint32_t(move.player_movePosition_x)       )
                          & ( uint32_t(move.player_movePosition_y) <<  8 )
                          & ( uint32_t(move.barrier_position_x   ) << 16 )
                          & ( uint32_t(move.barrier_position_y   ) << 24 );
    return ( hash<uint32_t>()(movePosition) ^ move.isBarrierPlacement ^ move.player_isWhite ^ move.barrier_isHorizontal);
  }
};

class QuoridorBoard {
public:
  QuoridorBoard();
  ~QuoridorBoard();

  BarrierGrid     barriers;
  array<Player,2> players;
  BoardConfig     config;
  bool            whitesTurn;

  bool operator==(const QuoridorBoard& other) const;

  void                       startInteractiveMode();
  void                       print(PrintArgs args = PrintArgs());

  Array2D<Array2D<bool,9>,9> get_adjacencyTables();
  BarrierGrid                get_legalBarrierPlacements();
  unordered_set<Move>        get_legalMoves(bool playerIsWhite);

  void doMove(Move move);
  void undoMove(Move move);
  void debug_printMove(Move move, uint8_t indent = 0);

  void debug_checkInvalidStates();
  void debug_clearBarriers();
  void debug_resetPlayers();
  void debug_setCornerHorizontalBarriers();
  void debug_setCornerVerticalBarriers();
  void debug_setRandomBarriers();
  void debug_setRandomPlayerPositions();
  void debug_printAdjacentTable(uint8_t x, uint8_t y);
};

template <> struct hash<QuoridorBoard> {
  size_t operator()(const QuoridorBoard& board) const {
    uint64_t barriers_horizontal    = array8x8bool_to_uint64(board.barriers.horizontal);
    uint64_t barriers_vertical      = array8x8bool_to_uint64(board.barriers.vertical);
    uint8_t  player_0_position_x    = board.players[0].position_x;
    uint8_t  player_0_position_y    = board.players[0].position_y;
    uint8_t  player_0_barriers_left = board.players[0].barriers_left;
    uint8_t  player_1_position_x    = board.players[1].position_x;
    uint8_t  player_1_position_y    = board.players[1].position_y;
    uint8_t  player_1_barriers_left = board.players[1].barriers_left;
    uint64_t players = ( uint64_t(player_0_position_x   )       )
                     & ( uint64_t(player_0_position_y   ) <<  8 )
                     & ( uint64_t(player_0_barriers_left) << 16 )
                     & ( uint64_t(player_1_position_x   ) << 32 )
                     & ( uint64_t(player_1_position_y   ) << 40 )
                     & ( uint64_t(player_1_barriers_left) << 48 );
    return (  hash<uint64_t>()(barriers_horizontal)
            ^ hash<uint64_t>()(barriers_vertical)
            ^ hash<uint64_t>()(players) );
  }
};
