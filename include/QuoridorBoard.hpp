#pragma once

#include <array>
#include <cstdint>
#include <iostream>

using namespace std;

struct BarrierGrid {
  array<array<bool, 8>, 8> vertical;
  array<array<bool, 8>, 8> horizontal;
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