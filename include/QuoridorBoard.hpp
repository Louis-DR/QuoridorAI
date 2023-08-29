#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <random>
#include <algorithm>

using namespace std;

template <class T, size_t N> using Array2D = array<array<T,N>,N>;

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
};

struct PrintArgs {
  bool    adjacent_table_enable = false;
  uint8_t adjacent_table_x      = 0;
  uint8_t adjacent_table_y      = 0;
  bool    legal_barrier_enable  = false;
};

class QuoridorBoard {
public:
  QuoridorBoard();
  ~QuoridorBoard();

  BarrierGrid     barriers;
  array<Player,2> players;
  BoardConfig     config;

  void print(PrintArgs args = {});
  Array2D<Array2D<bool,9>,9> get_adjacencyTables();
  BarrierGrid get_legalBarrierPlacements();

  void startInteractiveMode();

  void debug_checkInvalidStates();
  void debug_clearBarriers();
  void debug_resetPlayers();
  void debug_setCornerHorizontalBarriers();
  void debug_setCornerVerticalBarriers();
  void debug_setRandomBarriers();
  void debug_setRandomPlayerPositions();
  void debug_printAdjacentTable(uint8_t x, uint8_t y);
};