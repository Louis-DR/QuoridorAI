#include "QuoridorBoard.hpp"

QuoridorBoard::QuoridorBoard() {
  // Init barrier tables to zeros
  barriers.vertical.fill({});
  barriers.horizontal.fill({});

  // Init position of both players
  players[0].position_x = 4;
  players[0].position_y = 0;
  players[1].position_x = 4;
  players[1].position_y = 8;

  // Init barrier counts to 10 for both players
  players[0].barriers_left = 10;
  players[1].barriers_left = 10;
}

QuoridorBoard::~QuoridorBoard() {}

void QuoridorBoard::print() {
  // Top row
  cout << "┏━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┓\n";
  // Iterate over rows from top to bottom
  for (int y = 8; y >= 0; --y) {
    // Left edge
    cout << "┃";
    // Iterate over columns from left to right
    for (int x = 0; x < 9; ++x) {
      // Draw player piece
      if (x == players[0].position_x && y == players[0].position_y) cout << " ○ ";
      else if (x == players[1].position_x && y == players[1].position_y) cout << " ● ";
      else cout << "   ";
      // If not at the last column, draw the veritcal barrier
      if (x != 8) {
        if ((y != 0 && barriers.vertical[x][y-1]) || (y != 8 && barriers.vertical[x][y])) cout << "┃";
        else cout << " ";
      }
    }
    // Right edge
    cout << "┃\n";

    // Draw the horizontal edge
    if (y != 0) {
      // Left edge
      cout << "┠";
      // Iterate over columns from left to right
      for (int x = 0; x < 9; ++x) {
        // If not at the last column, draw the veritcal barrier
        if (x != 8 && ((x != 0 && barriers.horizontal[x-1][y-1]) || (x != 8 && barriers.horizontal[x][y-1]))) cout << "╼━╾";
        else cout << "╴ ╶";
        // Draw the cross point
        if (x != 8) cout << "┼";
      }
      // Right edge
      cout << "┨\n";
    }
  }
  // Bottom row
  cout << "┗━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┛\n" << endl;
}

Array2D<Array2D<bool,9>,9> QuoridorBoard::get_adjacencyTables() {
  Array2D<Array2D<bool,9>,9> adjacencyTables;

  // Generate table without any obstacle
  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 9; y++) {
      // Init to no move possible
      adjacencyTables[x][y].fill({});

      // Immediate adjacent cells
      if (x != 0) adjacencyTables[x][y][x-1][y  ] = true;
      if (x != 8) adjacencyTables[x][y][x+1][y  ] = true;
      if (y != 0) adjacencyTables[x][y][x  ][y-1] = true;
      if (y != 8) adjacencyTables[x][y][x  ][y+1] = true;
    }
  }

  // Compute barriers
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (barriers.vertical[x][y]) {
        adjacencyTables[x  ][y  ][x+1][y  ] = true;
        adjacencyTables[x+1][y  ][x  ][y  ] = true;
        adjacencyTables[x  ][y+1][x+1][y+1] = true;
        adjacencyTables[x+1][y+1][x  ][y+1] = true;
      }
      if (barriers.horizontal[x][y]) {
        adjacencyTables[x  ][y  ][x  ][y+1] = true;
        adjacencyTables[x  ][y+1][x  ][y  ] = true;
        adjacencyTables[x+1][y  ][x+1][y+1] = true;
        adjacencyTables[x+1][y+1][x+1][y  ] = true;
      }
    }
  }

  return adjacencyTables;
}

void QuoridorBoard::debug_clearBarriers() {
  barriers.vertical.fill({});
  barriers.horizontal.fill({});
}

void QuoridorBoard::debug_setCornerHorizontalBarriers() {
  debug_clearBarriers();

  barriers.horizontal[0][0] = true;
  barriers.horizontal[0][7] = true;
  barriers.horizontal[7][0] = true;
  barriers.horizontal[7][7] = true;
}

void QuoridorBoard::debug_setCornerVerticalBarriers() {
  debug_clearBarriers();

  barriers.vertical[0][0] = true;
  barriers.vertical[0][7] = true;
  barriers.vertical[7][0] = true;
  barriers.vertical[7][7] = true;
}

void QuoridorBoard::debug_setRandomBarriers() {
  debug_clearBarriers();

  random_device randseed;
  mt19937 randgen{randseed()};
  uniform_int_distribution<> rand_0_20(0,20);
  uniform_int_distribution<> rand_0_7(0,7);
  uniform_int_distribution<> rand_0_1(0,1);

  uint8_t number_barriers = rand_0_20(randgen);

  for (int barrier = 0; barrier < number_barriers; ++barrier) {
    bool orientation = rand_0_1(randgen);
    size_t x = rand_0_7(randgen);
    size_t y = rand_0_7(randgen);
    if (orientation) barriers.horizontal[x][y] = true;
    else barriers.vertical[x][y] = true;
  }
}

void QuoridorBoard::debug_setRandomPlayerPositions() {
  random_device randseed;
  mt19937 randgen{randseed()};
  uniform_int_distribution<> rand_0_8(0,8);

  players[0].position_x = rand_0_8(randgen);
  players[0].position_y = rand_0_8(randgen);
  players[1].position_x = rand_0_8(randgen);
  players[1].position_y = rand_0_8(randgen);
}