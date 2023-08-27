#pragma once

#include <array>
#include <cstdint>

using namespace std;

class QuoridorGame {

array<array<bool, 8>, 8> barriers_vertical, barriers_horizontal;
array<array<array<bool, 9>, 9>, 2> player_position;
array<uint8_t, 2> player_barriers_left;

public:
    QuoridorGame();
    ~QuoridorGame();
};