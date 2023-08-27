#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <random>

using namespace std;

template <class T, size_t N> using Array2D = array<array<T,N>,N>;

struct BarrierGrid {
  Array2D<bool,8> vertical;
  Array2D<bool,8> horizontal;
};

struct Player {
  Array2D<bool,9> position;
  uint8_t barriers_left;
};

class QuoridorBoard {

  BarrierGrid barriers;
  array<Player,2> players;

public:
  QuoridorBoard();
  ~QuoridorBoard();

  void print();
  Array2D<Array2D<bool,9>,9> get_adjacencyTables();

  void debug_set_randomBarriers();
  void debug_set_randomPlayerPositions();
};