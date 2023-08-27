#include "QuoridorBoard.hpp"

QuoridorBoard::QuoridorBoard() {
  // Init barrier tables to zeros
  barriers.vertical.fill({});
  barriers.horizontal.fill({});

  // Init position of both players
  players[0].position.fill({});
  players[0].position[4][0] = true;
  players[1].position.fill({});
  players[1].position[4][8] = true;

  // Init barrier counts to zero for both players
  players[0].barriers_left = 0;
  players[1].barriers_left = 0;
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
      if (players[0].position[x][y]) cout << " ○ ";
      else if (players[1].position[x][y]) cout << " ● ";
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
        if (x != 8 && ((x != 0 && barriers.horizontal[x-1][y]) || (x != 8 && barriers.horizontal[x][y]))) cout << "╼━╾";
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
