#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <random>

using namespace std;

template <class T, size_t N>
using Array2D = array<array<T, N>, N>;

struct BarrierGrid {
  Array2D<bool, 8> vertical;
  Array2D<bool, 8> horizontal;
};

struct Player
{
  uint8_t position_x;
  uint8_t position_y;
  uint8_t barriers_left;
};

struct BoardConfig
{
  bool interactive_auto_print = false;
};

class QuoridorBoard
{
public:
  QuoridorBoard();
  ~QuoridorBoard();
  BarrierGrid barriers;
  array<Player, 2> players;
  BoardConfig config;

  void print(bool adjacent_table_enable = false, size_t adjacent_table_x = 0, size_t adjacent_table_y = 0);
  Array2D<Array2D<bool, 9>, 9> get_adjacencyTables();

  void startInteractiveMode();

  void debug_checkInvalidStates();
  void debug_clearBarriers();
  void debug_resetPlayers();
  void debug_setCornerHorizontalBarriers();
  void debug_setCornerVerticalBarriers();
  void debug_setRandomBarriers();
  void debug_setRandomPlayerPositions();
  void debug_printAdjacentTable(size_t x, size_t y);
};