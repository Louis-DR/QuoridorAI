#pragma once

#include <array>
#include <cstdint>

using namespace std;

struct BarrierGrid {
  array<array<bool, 8>, 8> vertical;
  array<array<bool, 8>, 8> horizontal;
};

struct Player {
  array<array<bool, 9>, 9> position;
  uint8_t barriers_left;
};

class QuoridorGame {

  BarrierGrid barriers;
  array<Player, 2> players;

public:
  QuoridorGame();
  ~QuoridorGame();

  void print();
};