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
  std::cout << "      0   1   2   3   4   5   6   7    \n";
  std::cout << "  ┏━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┓\n";
  // Iterate over rows from top to bottom
  for (int y = 8; y >= 0; --y) {
    // Left edge
    std::cout << y <<" ┃";
    // Iterate over columns from left to right
    for (int x = 0; x < 9; ++x) {
      // Draw player piece
      if (x == players[0].position_x && y == players[0].position_y) std::cout << " ○ ";
      else if (x == players[1].position_x && y == players[1].position_y) std::cout << " ● ";
      else std::cout << "   ";
      // If not at the last column, draw the veritcal barrier
      if (x != 8) {
        if ((y != 0 && barriers.vertical[x][y-1]) || (y != 8 && barriers.vertical[x][y])) std::cout << "┃";
        else std::cout << " ";
      }
    }
    // Right edge
    std::cout << "┃\n";

    // Draw the horizontal edge
    if (y != 0) {
      // Left edge
      std::cout << "  ┠";
      // Iterate over columns from left to right
      for (int x = 0; x < 9; ++x) {
        // Draw the veritcal barrier
        if ((x != 0 && barriers.horizontal[x-1][y-1]) || (x != 8 && barriers.horizontal[x][y-1])) std::cout << "╼━╾";
        else std::cout << "╴ ╶";
        // Draw the cross point
        if (x != 8) std::cout << "┼";
      }
      // Right edge
      std::cout << "┨ " << y-1 << "\n";
    }
  }
  // Bottom row
  std::cout << "  ┗━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┛\n";
  std::cout << "    0   1   2   3   4   5   6   7   8  " << endl;
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
      if (barriers.horizontal[x][y]) {
        adjacencyTables[x  ][y  ][x  ][y+1] = true;
        adjacencyTables[x  ][y+1][x  ][y  ] = true;
        adjacencyTables[x+1][y  ][x+1][y+1] = true;
        adjacencyTables[x+1][y+1][x+1][y  ] = true;
      }
      if (barriers.vertical[x][y]) {
        adjacencyTables[x  ][y  ][x+1][y  ] = true;
        adjacencyTables[x+1][y  ][x  ][y  ] = true;
        adjacencyTables[x  ][y+1][x+1][y+1] = true;
        adjacencyTables[x+1][y+1][x  ][y+1] = true;
      }
    }
  }

  return adjacencyTables;
}

void QuoridorBoard::startInteractiveMode() {
  std::cout << "Starting interactive mode." << endl;
  string cmd_str;
  stringstream cmd_stream;
  vector<string> cmd_split;
  string cmd_split_el;
  string cmd_op;
  size_t cmd_len;

  while (true) {
    // Get user command
    getline(cin, cmd_str);

    // Split command by spaces
    cmd_stream = stringstream{cmd_str};
    cmd_split.clear();
    while (getline(cmd_stream, cmd_split_el, ' '))
      cmd_split.push_back(cmd_split_el);

    // Decoding command, could be optimized with constexpr hashing and switch
    cmd_op = cmd_split[0];
    cmd_len = cmd_split.size();
    if (cmd_op == "quit" || cmd_op == "exit") {
      std::cout << "Quitting interactive mode." << endl;
      break;
    } else if (cmd_op == "config") {
      if (cmd_len < 3) {
        std::cout << "ERROR: Not enough arguments for command 'config'." << endl;
        continue;
      }
      string cmd_config_key = cmd_split[1];
      string cmd_config_val = cmd_split[2];
      if (cmd_config_key == "auto_print") {
        if (cmd_config_val == "true" || cmd_config_val == "1") {
          config.interactive_auto_print = true;
        } else if (cmd_config_val == "false" || cmd_config_val == "0") {
          config.interactive_auto_print = false;
        } else {
          std::cout << "ERROR: Invalid value '" << cmd_config_val << "' for configuration option '" << cmd_config_key << "'." << std::endl;
        }
      }
    } else if (cmd_op == "check") {
      debug_checkInvalidStates();
    } else if (cmd_op == "reset") {
      debug_clearBarriers();
      debug_resetPlayers();
    } else if (cmd_op == "print") {
      print();
    } else if (cmd_op == "random") {
      if (cmd_len > 1) {
        string cmd_arg = cmd_split[1];
        if (cmd_arg == "players") {
          debug_setRandomPlayerPositions();
        } else if (cmd_arg == "barriers") {
          debug_setRandomBarriers();
        } else if (cmd_arg == "all") {
          debug_setRandomPlayerPositions();
          debug_setRandomBarriers();
        } else {
          std::cout << "ERROR: Unknown argument '" << cmd_arg << "' for command 'random'." << endl;
        }
      } else {
        debug_setRandomPlayerPositions();
        debug_setRandomBarriers();
      }
    } else if (cmd_op == "barrier") {
      if (cmd_len < 2) {
        std::cout << "ERROR: Not enough arguments for command 'barrier'." << endl;
        continue;
      }
      string cmd_subop = cmd_split[1];
      if (cmd_subop == "add") {
        if (cmd_len < 5) {
          std::cout << "ERROR: Not enough arguments for command 'barrier add'." << endl;
          continue;
        }
        string cmd_direction = cmd_split[2];
        auto cmd_x = stoi(cmd_split[3]);
        auto cmd_y = stoi(cmd_split[4]);
        if (cmd_direction == "horizontal" || cmd_direction == "h") {
          barriers.horizontal[cmd_x][cmd_y] = true;
        } else if (cmd_direction == "vertical" || cmd_direction == "v") {
          barriers.vertical[cmd_x][cmd_y] = true;
        } else {
          std::cout << "ERROR: Invalid direction '" << cmd_direction << "' for command 'barrier add'." << endl;
        }
      } else if (cmd_subop == "remove") {
        if (cmd_len < 5) {
          std::cout << "ERROR: Not enough arguments for command 'barrier remove'." << endl;
          continue;
        }
        string cmd_direction = cmd_split[2];
        auto cmd_x = stoi(cmd_split[3]);
        auto cmd_y = stoi(cmd_split[4]);
        if (cmd_direction == "horizontal" || cmd_direction == "h") {
          barriers.horizontal[cmd_x][cmd_y] = false;
        } else if (cmd_direction == "vertical" || cmd_direction == "v") {
          barriers.vertical[cmd_x][cmd_y] = false;
        } else {
          std::cout << "ERROR: Invalid direction '" << cmd_direction << "' for command 'barrier remove'." << endl;
        }
      } else if (cmd_subop == "clear") {
        debug_clearBarriers();
      } else if (cmd_subop == "random") {
        debug_setRandomBarriers();
      } else {
        std::cout << "ERROR: Unknown sub-command '" << cmd_subop << "' for command 'barrier'." << endl;
      }
    } else if (cmd_op == "player") {
      if (cmd_len < 2) {
        std::cout << "ERROR: Not enough arguments for command 'player'." << endl;
        continue;
      }
      string cmd_subop = cmd_split[1];
      if (cmd_subop == "move") {
        if (cmd_len < 5) {
          std::cout << "ERROR: Not enough arguments for command 'barrier move'." << endl;
          continue;
        }
        string cmd_player_select = cmd_split[2];
        auto cmd_x = stoi(cmd_split[3]);
        auto cmd_y = stoi(cmd_split[4]);
        if (cmd_player_select == "white" || cmd_player_select == "w" || cmd_player_select == "0") {
          players[0].position_x = cmd_x;
          players[0].position_y = cmd_y;
        } else if (cmd_player_select == "black" || cmd_player_select == "b" || cmd_player_select == "1") {
          players[1].position_x = cmd_x;
          players[1].position_y = cmd_y;
        } else {
          std::cout << "ERROR: Invalid player selector '" << cmd_player_select << "' for command 'player move'." << endl;
        }
      } else if (cmd_subop == "reset") {
        debug_resetPlayers();
      } else if (cmd_subop == "random") {
        debug_setRandomPlayerPositions();
      } else {
        std::cout << "ERROR: Unknown sub-command '" << cmd_subop << "' for command 'player'." << endl;
      }
    } else {
      std::cout << "ERROR: Unknown command '" << cmd_op << "'." << endl;
    }
    if (config.interactive_auto_print) print();
  }
  std::cout << "Interactive mode finished." << endl;
}

void QuoridorBoard::debug_checkInvalidStates() {
  std::cout << "DEBUG: Checking for invalid board states." << endl;

  if ((players[0].position_x == 0 && players[1].position_x) && (players[0].position_y == players[1].position_y))
    std::cout << "ERROR: Invalid board state, players have the same position " << players[0].position_x << "-" << players[0].position_y << "." << endl;

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (barriers.horizontal[x][y]) {
        if (barriers.vertical[x][y])
          std::cout << "ERROR: Invalid board state, both vertical and horizontal barriers at position " << x << "-" << y << endl;
        if (x != 7 && barriers.horizontal[x+1][y])
          std::cout << "ERROR: Invalid board state, overlapping horizontal barriers at positions " << x << "-" << y << " and " << x+1 << "-" << y << endl;
      }
      else if (barriers.vertical[x][y]) {
        if (y != 7 && barriers.vertical[x][y+1])
          std::cout << "ERROR: Invalid board state, overlapping vertical barriers at positions " << x << "-" << y << " and " << x << "-" << y+1 << endl;
      }
    }
  }
}

void QuoridorBoard::debug_clearBarriers() {
  std::cout << "DEBUG: Clearing the barriers." << endl;
  barriers.vertical.fill({});
  barriers.horizontal.fill({});
}

void QuoridorBoard::debug_resetPlayers() {
  std::cout << "DEBUG: Resetting the players." << endl;

  players[0].position_x = 4;
  players[0].position_y = 0;
  players[1].position_x = 4;
  players[1].position_y = 8;

  players[0].barriers_left = 10;
  players[1].barriers_left = 10;
}

void QuoridorBoard::debug_setCornerHorizontalBarriers() {
  debug_clearBarriers();

  std::cout << "DEBUG: Placing only horizontal barriers at the corners." << endl;

  barriers.horizontal[0][0] = true;
  barriers.horizontal[0][7] = true;
  barriers.horizontal[7][0] = true;
  barriers.horizontal[7][7] = true;
}

void QuoridorBoard::debug_setCornerVerticalBarriers() {
  debug_clearBarriers();

  std::cout << "DEBUG: Placing only vertical barriers at the corners." << endl;

  barriers.vertical[0][0] = true;
  barriers.vertical[0][7] = true;
  barriers.vertical[7][0] = true;
  barriers.vertical[7][7] = true;
}

void QuoridorBoard::debug_setRandomBarriers() {
  debug_clearBarriers();

  std::cout << "DEBUG: Placing barriers at random locations." << endl;

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
  std::cout << "DEBUG: Placing the players at random positions." << endl;

  random_device randseed;
  mt19937 randgen{randseed()};
  uniform_int_distribution<> rand_0_8(0,8);

  players[0].position_x = rand_0_8(randgen);
  players[0].position_y = rand_0_8(randgen);
  players[1].position_x = rand_0_8(randgen);
  players[1].position_y = rand_0_8(randgen);
}