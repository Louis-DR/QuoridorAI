#pragma once

#include <array>
#include <cstdint>
#include <iostream>

using namespace std;

template <size_t N> using Array2D = array<array<bool, N>, N>;

struct BarrierGrid {
  Array2D<8> vertical;
  Array2D<8> horizontal;
};

struct Player {
  array<array<bool, 9>, 9> position;
  uint8_t barriers_left;
};

class QuoridorBoard {

  BarrierGrid barriers;
  array<Player, 2> players;

public:
  QuoridorBoard();
  ~QuoridorBoard();

  void print();
  void get_adjacencyTable();
};