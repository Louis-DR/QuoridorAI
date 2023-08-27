#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <random>

using namespace std;

template <class T, size_t N> using Array2D = array<array<T, N>, N>;

struct BarrierGrid {
  Array2D<bool,8> vertical;
  Array2D<bool,8> horizontal;
};

struct Player {
  uint8_t position_x;
  uint8_t position_y;
  uint8_t barriers_left;
};

class QuoridorBoard {
public:
  QuoridorBoard();
  ~QuoridorBoard();
  BarrierGrid barriers;
  array<Player,2> players;

  void print();
  Array2D<Array2D<bool, 9>, 9> get_adjacencyTables();

  void debug_checkInvalidStates();
  void debug_clearBarriers();
  void debug_setCornerHorizontalBarriers();
  void debug_setCornerVerticalBarriers();
  void debug_setRandomBarriers();
  void debug_setRandomPlayerPositions();
};