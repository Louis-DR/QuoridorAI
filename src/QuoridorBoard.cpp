#include "QuoridorBoard.hpp"
#include "QuoridorAgent.hpp"

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

  // White is first player
  whitesTurn = true;
}

QuoridorBoard::~QuoridorBoard() {}

uint64_t array8x8bool_to_uint64(Array2D<bool,8> array8x8bool) {
  uint64_t uint64   = 0;
  size_t   bitcount = 0;
  for (size_t x = 0; x < 8; x++) {
    for (size_t y = 0; y < 8; y++) {
      uint64 &= array8x8bool[x][y] << bitcount;
      bitcount++;
    }
  }
  return uint64;
}

bool QuoridorBoard::operator==(const QuoridorBoard& other) const {
  return (   array8x8bool_to_uint64(this->barriers.horizontal) == array8x8bool_to_uint64(other.barriers.horizontal)
          && array8x8bool_to_uint64(this->barriers.vertical)   == array8x8bool_to_uint64(other.barriers.vertical)
          && this->players[0].position_x                       == other.players[0].position_x
          && this->players[0].position_y                       == other.players[0].position_y
          && this->players[0].barriers_left                    == other.players[0].barriers_left
          && this->players[1].position_x                       == other.players[1].position_x
          && this->players[1].position_y                       == other.players[1].position_y
          && this->players[1].barriers_left                    == other.players[1].barriers_left );
}

bool Move::operator==(const Move& other) const {
  return (   this->isBarrierPlacement        == other.isBarrierPlacement
          && this->player_isWhite            == other.player_isWhite
          && this->player_originalPosition_x == other.player_originalPosition_x
          && this->player_originalPosition_y == other.player_originalPosition_y
          && this->player_movePosition_x     == other.player_movePosition_x
          && this->player_movePosition_y     == other.player_movePosition_y
          && this->barrier_isHorizontal      == other.barrier_isHorizontal
          && this->barrier_position_x        == other.barrier_position_x
          && this->barrier_position_y        == other.barrier_position_y );
}

void QuoridorBoard::startInteractiveMode() {
  std::cout << "Starting interactive mode." << endl;

  string cmd_str;
  stringstream cmd_stream;
  vector<string> cmd_split;
  string cmd_split_el;
  string cmd_op;
  size_t cmd_len;

  // Endless loop to read commands
  while (true) {
    // Get user command
    getline(cin, cmd_str);

    // Split command by spaces
    cmd_stream = stringstream{ cmd_str };
    cmd_split.clear();
    while (getline(cmd_stream, cmd_split_el, ' '))
      cmd_split.push_back(cmd_split_el);

    // Decoding command, could be optimized with constexpr hashing and switch
    cmd_op = cmd_split[0];
    cmd_len = cmd_split.size();

    // Quit interactive mode
    if (cmd_op == "quit" || cmd_op == "exit") {
      std::cout << "Quitting interactive mode." << endl;
      break;
    }

    // Change configuration of the board
    else if (cmd_op == "config") {
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
      } else if (cmd_config_key == "dot_grid") {
        if (cmd_config_val == "true" || cmd_config_val == "1") {
          config.print_dot_grid = true;
        } else if (cmd_config_val == "false" || cmd_config_val == "0") {
          config.print_dot_grid = false;
        } else {
          std::cout << "ERROR: Invalid value '" << cmd_config_val << "' for configuration option '" << cmd_config_key << "'." << std::endl;
        }
      } else if (cmd_config_key == "ai_depth") {
        config.ai_minimax_depth = stoi(cmd_config_val);
      } else {
        std::cout << "ERROR: Unknown configuration option '" << cmd_config_key << "'." << std::endl;
      }
    }

    // Check invalid states
    else if (cmd_op == "check") {
      debug_checkInvalidStates();
    }

    // Reset the board
    else if (cmd_op == "reset") {
      debug_clearBarriers();
      debug_resetPlayers();
    }

    // Print the board
    else if (cmd_op == "print") {
      print();
    }

    // Set random state
    else if (cmd_op == "random") {
      // If provided extra arg
      if (cmd_len > 1) {
        string cmd_arg = cmd_split[1];
        // Only random player positions
        if (cmd_arg == "players") {
          debug_setRandomPlayerPositions();
        }
        // Only random barriers
        else if (cmd_arg == "barriers") {
          debug_setRandomBarriers();
        }
        // Random players and barriers
        else if (cmd_arg == "all") {
          debug_setRandomPlayerPositions();
          debug_setRandomBarriers();
        }
        // Invalid argument
        else {
          std::cout << "ERROR: Unknown argument '" << cmd_arg << "' for command 'random'." << endl;
        }
      }
      // Random players and barriers
      else {
        debug_setRandomPlayerPositions();
        debug_setRandomBarriers();
      }
    }

    // Modify barriers
    else if (cmd_op == "barrier") {
      // Must have a sub-command
      if (cmd_len < 2) {
        std::cout << "ERROR: Not enough arguments for command 'barrier'." << endl;
        continue;
      }
      string cmd_subop = cmd_split[1];
      // Place a new barrier
      if (cmd_subop == "add") {
        // Must provide orientation and coordinates of the barrier
        if (cmd_len < 5) {
          std::cout << "ERROR: Not enough arguments for command 'barrier add'." << endl;
          continue;
        }
        string cmd_orientation = cmd_split[2];
        auto cmd_x = stoi(cmd_split[3]);
        auto cmd_y = stoi(cmd_split[4]);
        if (cmd_orientation == "horizontal" || cmd_orientation == "h") {
          barriers.horizontal[cmd_x][cmd_y] = true;
        } else if (cmd_orientation == "vertical" || cmd_orientation == "v") {
          barriers.vertical[cmd_x][cmd_y] = true;
        } else {
          std::cout << "ERROR: Invalid orientation '" << cmd_orientation << "' for command 'barrier add'." << endl;
        }
      // Remove an existing barrier
      } else if (cmd_subop == "remove") {
        // Must provide orientation and coordinates of the barrier
        if (cmd_len < 5) {
          std::cout << "ERROR: Not enough arguments for command 'barrier remove'." << endl;
          continue;
        }
        string cmd_orientation = cmd_split[2];
        auto cmd_x = stoi(cmd_split[3]);
        auto cmd_y = stoi(cmd_split[4]);
        if (cmd_orientation == "horizontal" || cmd_orientation == "h") {
          barriers.horizontal[cmd_x][cmd_y] = false;
        } else if (cmd_orientation == "vertical" || cmd_orientation == "v") {
          barriers.vertical[cmd_x][cmd_y] = false;
        } else {
          std::cout << "ERROR: Invalid orientation '" << cmd_orientation << "' for command 'barrier remove'." << endl;
        }
      // Remove all barriers
      } else if (cmd_subop == "clear") {
        debug_clearBarriers();
      // Set random barriers
      } else if (cmd_subop == "random") {
        debug_setRandomBarriers();
      // Invalid sub-command
      } else {
        std::cout << "ERROR: Unknown sub-command '" << cmd_subop << "' for command 'barrier'." << endl;
      }
    }

    // Modify players
    else if (cmd_op == "player") {
      // Must provide sub-command
      if (cmd_len < 2) {
        std::cout << "ERROR: Not enough arguments for command 'player'." << endl;
        continue;
      }
      string cmd_subop = cmd_split[1];
      // Move player
      if (cmd_subop == "move") {
        // Must provide player selector and target coordinates
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
        }
        else if (cmd_player_select == "black" || cmd_player_select == "b" || cmd_player_select == "1") {
          players[1].position_x = cmd_x;
          players[1].position_y = cmd_y;
        }
        else {
          std::cout << "ERROR: Invalid player selector '" << cmd_player_select << "' for command 'player move'." << endl;
        }
      // Reset player positions
      } else if (cmd_subop == "reset") {
        debug_resetPlayers();
      // Move players to random positions
      } else if (cmd_subop == "random") {
        debug_setRandomPlayerPositions();
      // Invalid sub-command
      } else {
        std::cout << "ERROR: Unknown sub-command '" << cmd_subop << "' for command 'player'." << endl;
      }
    }

    // Print the board with possible destinations from a source position
    else if (cmd_op == "adjacent") {
      // Must provide the source position
      if (cmd_len < 3) {
        std::cout << "ERROR: Not enough arguments for command 'adjacent'." << endl;
        continue;
      }
      uint8_t cmd_x = stoi(cmd_split[1]);
      uint8_t cmd_y = stoi(cmd_split[2]);
      print({.adjacent_table_enable=true, .adjacent_table_x=cmd_x, .adjacent_table_y=cmd_y});
    }

    // Print the board with possible destinations from a source position
    else if (cmd_op == "legal") {
      // Must provide sub-command
      if (cmd_len < 2) {
        std::cout << "ERROR: Not enough arguments for command 'legal'." << endl;
        continue;
      }
      string cmd_subop = cmd_split[1];
      // Show legal barrier placements
      if (cmd_subop == "barrier") {
        print({.legal_barrier_enable=true});
      // Invalid sub-command
      } else {
        std::cout << "ERROR: Unknown sub-command '" << cmd_subop << "' for command 'legal'." << endl;
      }
    }

    // Distance to reach winning line
    else if (cmd_op == "distance") {
      QuoridorAgent agent{};

      // Must provide sub-command
      if (cmd_len < 2) {
        std::cout << "Distance for white player : " << +agent.getMinDistancePlayer(*this, true) << endl;
        std::cout << "Distance for black player : " << +agent.getMinDistancePlayer(*this, false) << endl;
      } else {
        string cmd_player_select = cmd_split[1];
        if (cmd_player_select == "white" || cmd_player_select == "w" || cmd_player_select == "0")
          std::cout << "Distance for white player : " << +agent.getMinDistancePlayer(*this, true) << endl;
        else if (cmd_player_select == "black" || cmd_player_select == "b" || cmd_player_select == "1")
          std::cout << "Distance for black player : " << +agent.getMinDistancePlayer(*this, false) << endl;
        else
          std::cout << "ERROR: Invalid player selector '" << cmd_player_select << "' for command 'player move'." << endl;
      }
    }

    // Print the board with possible destinations from a source position
    else if (cmd_op == "ai") {
      // Must provide sub-command
      if (cmd_len < 2) {
        std::cout << "ERROR: Not enough arguments for command 'ai'." << endl;
        continue;
      }
      string cmd_subop = cmd_split[1];
      // Play next move by the computer
      if (cmd_subop == "next") {
        uint8_t numberMoves = (cmd_len > 2) ? stoi(cmd_split[2]) : 1;
        QuoridorAgent agent{};
        for (size_t i = 0; i < numberMoves; ++i) {
          Move bestMove = agent.get_bestMove(*this, get_legalMoves(whitesTurn), whitesTurn, config.ai_minimax_depth);
          doMove(bestMove);
          whitesTurn = !whitesTurn;
        }
      } else if (cmd_subop == "auto") {
        QuoridorAgent agent{};
        uint8_t numberMoves = (cmd_len > 2) ? stoi(cmd_split[2]) : 1;
        for (size_t i = 0; i < numberMoves; ++i) {
          if (agent.isGameFinished(*this, whitesTurn)) break;
          Move bestMove = agent.get_bestMove(*this, get_legalMoves(whitesTurn), whitesTurn, config.ai_minimax_depth);
          doMove(bestMove);
          whitesTurn = !whitesTurn;
          print();
          std::this_thread::sleep_for(1000ms);
        }
        cout << "Average search speed : " << agent.get_averageSearchSpeed() << "position/s" << std::endl;
      // Invalid sub-command
      } else {
        std::cout << "ERROR: Unknown sub-command '" << cmd_subop << "' for command 'ai'." << endl;
      }
    }

    // Invalid command
    else {
      std::cout << "ERROR: Unknown command '" << cmd_op << "'." << endl;
    }

    // Print the board after each command
    if (config.interactive_auto_print) print();
  }
  std::cout << "Interactive mode finished." << endl;
}

void QuoridorBoard::print(PrintArgs args) {
  // If drawing adjacency table, then compute it
  Array2D<bool,9> adjacencyTable;
  if (args.adjacent_table_enable)
    adjacencyTable = get_adjacencyTables()[args.adjacent_table_x][args.adjacent_table_y];

  // If drawing legal barrier moves, then compute them
  BarrierGrid legalBarrierPlacemenent;
  if (args.legal_barrier_enable)
    legalBarrierPlacemenent = get_legalBarrierPlacements();

  // Top row with black player barriers to place
  std::cout << "      0   1   2   3   4   5   6   7    " << (whitesTurn ? " " : "►") << "●:" << unsigned(players[1].barriers_left) << "\n";
  std::cout << "  ┏━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┓\n";
  // Iterate over rows from top to bottom
  for (int y = 8; y >= 0; --y) {
    // Left edge
    std::cout << y << " ┃";
    // Iterate over columns from left to right
    for (int x = 0; x < 9; ++x) {
      // Draw player piece
      if (x == players[0].position_x && y == players[0].position_y)
        std::cout << " ○ ";
      else if (x == players[1].position_x && y == players[1].position_y)
        std::cout << " ● ";
      else if (args.adjacent_table_enable && adjacencyTable[x][y])
        std::cout << " ⦾ ";
      else
        std::cout << "   ";
      // If not at the last column, draw the veritcal barrier
      if (x != 8) {
        if ((y != 0 && barriers.vertical[x][y-1]) || (y != 8 && barriers.vertical[x][y]))
          std::cout << "┃";
        else
          std::cout << " ";
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
        // Dot grid style
        if (config.print_dot_grid || args.legal_barrier_enable) {
          // Draw the horizontal barrier
          if (x != 0 && barriers.horizontal[x-1][y-1]) {
            std::cout << "━━╸";
            // Draw the cross point
            if (x != 8) {
              if (barriers.vertical[x][y-1])
                std::cout << "┃";
              else if (args.legal_barrier_enable && legalBarrierPlacemenent.vertical[x][y-1])
                std::cout << "│";
              else
                std::cout << "·";
            }
          }
          else if (x != 8 && barriers.horizontal[x][y-1]) {
            std::cout << "╺━━";
            // Draw the cross point
            if (x != 8)
              std::cout << "━";
          }
          else {
            std::cout << "   ";
            // Draw the cross point
            if (x != 8) {
              if (barriers.vertical[x][y-1])
                std::cout << "┃";
              else if (args.legal_barrier_enable)
                if (legalBarrierPlacemenent.horizontal[x][y-1])
                  if (legalBarrierPlacemenent.vertical[x][y-1])
                    std::cout << "┼";
                  else
                    std::cout << "─";
                else if (legalBarrierPlacemenent.vertical[x][y-1])
                  std::cout << "│";
                else
                  std::cout << "·";
              else
                std::cout << "·";
            }
          }
        // Cross grid style
        } else {
          // Draw the horizontal barrier
          if ((x != 0 && barriers.horizontal[x-1][y-1]) || (x != 8 && barriers.horizontal[x][y-1]))
            std::cout << "╼━╾";
          else
            std::cout << "╴ ╶";
          // Draw the cross point
          if (x != 8)
            std::cout << "┼";
        }
      }
      // Right edge
      std::cout << "┨ " << y - 1 << "\n";
    }
  }
  // Bottom row
  std::cout << "  ┗━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┛\n";
  std::cout << "    0   1   2   3   4   5   6   7   8  " << (whitesTurn ? "►" : " ") << "○:" << unsigned(players[0].barriers_left) << endl;
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

  // Compute blocking barrier
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (barriers.horizontal[x][y]) {
        adjacencyTables[x  ][y  ][x  ][y+1] = false;
        adjacencyTables[x  ][y+1][x  ][y  ] = false;
        adjacencyTables[x+1][y  ][x+1][y+1] = false;
        adjacencyTables[x+1][y+1][x+1][y  ] = false;
      }
      if (barriers.vertical[x][y]) {
        adjacencyTables[x  ][y  ][x+1][y  ] = false;
        adjacencyTables[x+1][y  ][x  ][y  ] = false;
        adjacencyTables[x  ][y+1][x+1][y+1] = false;
        adjacencyTables[x+1][y+1][x  ][y+1] = false;
      }
    }
  }

  uint8_t player1_x = players[0].position_x;
  uint8_t player1_y = players[0].position_y;
  uint8_t player2_x = players[1].position_x;
  uint8_t player2_y = players[1].position_y;

  // Add players to adjacency tables
  // if players are adjacent
  int8_t adversary_x;// Virtual player position
  int8_t adversary_y;
  for (uint8_t player_id= 0; player_id<2; player_id++){ // Considering player 1 and 2 alternately
        int8_t main_player_x = players[player_id].position_x;
        int8_t main_player_y = players[player_id].position_y;
    for (uint8_t direction=0; direction < 2; direction++){ // Considering East-West and North-South directions
    // Check there's no barrier between players (their adjacency is not blocked)
        for (int8_t orientation=-1; orientation<2; orientation++){ // Considering orientation ex: North-South and South-North

        // Check if behind cell exists and is not blocked
        if (direction == 0) { // East-West direction
          adversary_x = main_player_x + orientation;// Orientation
          adversary_y = main_player_y;
            if(adversary_x >= 0 && adversary_x <= 8 && adjacencyTables[adversary_x][adversary_y][main_player_x][main_player_y]){// Check if out-of-bounds
              int8_t delta = main_player_x - adversary_x;// Travel direction
              if (adjacencyTables[main_player_x][main_player_y][main_player_x + delta][main_player_y]) {// Check if presence of barrier
                adjacencyTables[adversary_x][adversary_y][main_player_x][main_player_y] = false;
                adjacencyTables[adversary_x][adversary_y][main_player_x + delta][main_player_y] = true;
              }
              else { // If presence of barrier
                for (int8_t i = -1; i < 2; i += 2) { // Check lateral cells access
                  if (main_player_y + i >= 0 && main_player_y + i <= 8 && adjacencyTables[main_player_x][main_player_y][main_player_x][main_player_y + i]) {
                    adjacencyTables[adversary_x][adversary_y][main_player_x][main_player_y] = false;
                    adjacencyTables[adversary_x][adversary_y][main_player_x][main_player_y + i] = true;
                  }
                }
              }
          }
        }
        else if (direction == 1) { // Considering North-South direction
          adversary_x = main_player_x;
          adversary_y = main_player_y + orientation;
          int8_t delta = main_player_y - adversary_y;// Travel direction
            if (adversary_y >= 0 && adversary_y <= 8 && adjacencyTables[adversary_x][adversary_y][main_player_x][main_player_y]){ // Check if out-of-bound
            if (main_player_y + delta >= 0 && delta + main_player_y <= 8 && adjacencyTables[main_player_x][main_player_y][main_player_x][delta + main_player_y]) {// Check if presence of barrier
              adjacencyTables[adversary_x][adversary_y][main_player_x][main_player_y] = false;
              adjacencyTables[adversary_x][adversary_y][main_player_x][main_player_y + delta] = true;
            }
            else {
              for (int8_t i = -1; i < 2; i += 2) { // Check lateral cells access
                if (main_player_x + i >= 0 && main_player_x + i <= 8 && adjacencyTables[main_player_x][main_player_y][main_player_x + i][main_player_y]) {
                  adjacencyTables[adversary_x][adversary_y][main_player_x][main_player_y] = false;
                  adjacencyTables[adversary_x][adversary_y][main_player_x + i][main_player_y] = true;
                }
              }
            }
          }
        }
      }
    }
  }

  return adjacencyTables;
}

BarrierGrid QuoridorBoard::get_legalBarrierPlacements() {
  // Init grid of legal barrier placement to all legal
  BarrierGrid legalBarrierPlacemenent;
  for (size_t x = 0; x < 8; ++x) {
    legalBarrierPlacemenent.horizontal[x].fill(true);
    legalBarrierPlacemenent.vertical[x].fill(true);
  }

  // Iterate over all existing barriers
  uint8_t numberBarriers = 0;
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (barriers.horizontal[x][y]) {
        numberBarriers++;
        legalBarrierPlacemenent.horizontal[x][y] = false;
        legalBarrierPlacemenent.vertical[x][y] = false;
        if (x > 0) legalBarrierPlacemenent.horizontal[x-1][y] = false;
        if (x < 7) legalBarrierPlacemenent.horizontal[x+1][y] = false;
      } else if (barriers.vertical[x][y]) {
        numberBarriers++;
        legalBarrierPlacemenent.horizontal[x][y] = false;
        legalBarrierPlacemenent.vertical[x][y] = false;
        if (y > 0) legalBarrierPlacemenent.vertical[x][y-1] = false;
        if (y < 7) legalBarrierPlacemenent.vertical[x][y+1] = false;
      }
    }
  }

  // If there are 4 or more barriers, then we have to check if no legal barrier could block the path
  // ToDo : fix, can block with 3 barriers on the edges and 2 barriers in the corners
  if (numberBarriers >= 2) {
    // Iterate over all candidate legal barrier placements
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        // If the barrier is sufficiently isolated, then it is not blocking
        // Sufficiently isolated means having no more than one neighbor barrier
        // Or a no neighbor barrier at all if also touching the edge of the grid
        bool is_blocking;
        uint8_t numberNeighbors;
        QuoridorAgent agent{};
        if (legalBarrierPlacemenent.horizontal[x][y]) {
          numberNeighbors = 0;
          if (x > 1          && barriers.horizontal[x-2][y  ]) numberNeighbors++;
          if (x < 6          && barriers.horizontal[x+2][y  ]) numberNeighbors++;
          if (x > 0 && y > 0 && barriers.vertical  [x-1][y-1]) numberNeighbors++;
          if (x > 0          && barriers.vertical  [x-1][y  ]) numberNeighbors++;
          if (x > 0 && y < 7 && barriers.vertical  [x-1][y+1]) numberNeighbors++;
          if (         y > 0 && barriers.vertical  [x  ][y-1]) numberNeighbors++;
          if (         y < 7 && barriers.vertical  [x  ][y+1]) numberNeighbors++;
          if (x < 7 && y > 0 && barriers.vertical  [x+1][y-1]) numberNeighbors++;
          if (x < 7          && barriers.vertical  [x+1][y  ]) numberNeighbors++;
          if (x < 7 && y < 7 && barriers.vertical  [x+1][y+1]) numberNeighbors++;

          if ((numberNeighbors > 1) || ((x == 0 || x == 7 || y == 0 || y == 7) && numberNeighbors > 0)) {
            // If placing this barrier blocks the game, then set the move as illegal
            barriers.horizontal[x][y] = true;
            is_blocking  = agent.getMinDistancePlayer(*this, true ) == 255; // White blocked
            is_blocking |= agent.getMinDistancePlayer(*this, false) == 255; // Black blocked
            if (is_blocking) legalBarrierPlacemenent.horizontal[x][y] = false;
            barriers.horizontal[x][y] = false;
          }
        }
        if (legalBarrierPlacemenent.vertical[x][y]) {
          numberNeighbors = 0;
          if (         y > 1 && barriers.vertical  [x  ][y-2]) numberNeighbors++;
          if (         y < 6 && barriers.vertical  [x  ][y+2]) numberNeighbors++;
          if (x > 0 && y > 0 && barriers.horizontal[x-1][y-1]) numberNeighbors++;
          if (         y > 0 && barriers.horizontal[x  ][y-1]) numberNeighbors++;
          if (x < 7 && y > 0 && barriers.horizontal[x+1][y-1]) numberNeighbors++;
          if (x > 0          && barriers.horizontal[x-1][y  ]) numberNeighbors++;
          if (x < 7          && barriers.horizontal[x+1][y  ]) numberNeighbors++;
          if (x > 0 && y < 7 && barriers.horizontal[x-1][y+1]) numberNeighbors++;
          if (         y < 7 && barriers.horizontal[x  ][y+1]) numberNeighbors++;
          if (x < 7 && y < 7 && barriers.horizontal[x+1][y+1]) numberNeighbors++;

          if ((numberNeighbors > 1) || ((x == 0 || x == 7 || y == 0 || y == 7) && numberNeighbors > 0)) {
            // If placing this barrier blocks the game, then set the move as illegal
            barriers.vertical[x][y] = true;
            is_blocking  = agent.getMinDistancePlayer(*this, true ) == 255; // White blocked
            is_blocking |= agent.getMinDistancePlayer(*this, false) == 255; // Black blocked
            if (is_blocking) legalBarrierPlacemenent.vertical[x][y] = false;
            barriers.vertical[x][y] = false;
          }
        }
      }
    }
  }

  return legalBarrierPlacemenent;
}

unordered_set<Move> QuoridorBoard::get_legalMoves(bool playerIsWhite) {
  unordered_set<Move> legalMoves;

  // Legal player movement from adjacency table
  Array2D<Array2D<bool,9>,9> adjacencyTables = get_adjacencyTables();
  uint8_t player_x = playerIsWhite ? players[0].position_x : players[1].position_x;
  uint8_t player_y = playerIsWhite ? players[0].position_y : players[1].position_y;
  for (size_t x = max(0, player_x - 2); x <= min(8, player_x + 3); x++) {
    for (size_t y = max(0, player_y -2); y <= min(8, player_y + 3); y++) {
      if (adjacencyTables[player_x][player_y][x][y]) {
        Move legalMove;
        legalMove.isBarrierPlacement        = false;
        legalMove.player_isWhite            = playerIsWhite;
        legalMove.player_originalPosition_x = player_x;
        legalMove.player_originalPosition_y = player_y;
        legalMove.player_movePosition_x     = x;
        legalMove.player_movePosition_y     = y;
        legalMoves.insert(legalMove);
      }
    }
  }

  // Legal barrier placement if the player has enough barriers left
  uint8_t player_barriersLeft = playerIsWhite ? players[0].barriers_left : players[1].barriers_left;
  if (player_barriersLeft != 0) {
    BarrierGrid legalBarrierPlacemenent = get_legalBarrierPlacements();
    for (size_t x = 0; x < 8; x++) {
      for (size_t y = 0; y < 8; y++) {
        if (legalBarrierPlacemenent.horizontal[x][y]) {
          Move legalMove;
          legalMove.isBarrierPlacement   = true;
          legalMove.player_isWhite       = playerIsWhite;
          legalMove.barrier_isHorizontal = true;
          legalMove.barrier_position_x   = x;
          legalMove.barrier_position_y   = y;
          legalMoves.insert(legalMove);
        }
        if (legalBarrierPlacemenent.vertical[x][y]) {
          Move legalMove;
          legalMove.isBarrierPlacement   = true;
          legalMove.player_isWhite       = playerIsWhite;
          legalMove.barrier_isHorizontal = false;
          legalMove.barrier_position_x   = x;
          legalMove.barrier_position_y   = y;
          legalMoves.insert(legalMove);
        }
      }
    }
  }

  return legalMoves;
}

void QuoridorBoard::doMove(Move move) {
  if (move.isBarrierPlacement) {
    if (move.barrier_isHorizontal) {
      barriers.horizontal[move.barrier_position_x][move.barrier_position_y] = true;
    } else {
      barriers.vertical[move.barrier_position_x][move.barrier_position_y] = true;
    }
    if (move.player_isWhite)
      players[0].barriers_left--;
    else
      players[1].barriers_left--;
  } else { // Else player movement
    if (move.player_isWhite) {
      players[0].position_x = move.player_movePosition_x;
      players[0].position_y = move.player_movePosition_y;
    } else {
      players[1].position_x = move.player_movePosition_x;
      players[1].position_y = move.player_movePosition_y;
    }
  }
}

void QuoridorBoard::undoMove(Move move) {
  if (move.isBarrierPlacement) {
    if (move.barrier_isHorizontal) {
      barriers.horizontal[move.barrier_position_x][move.barrier_position_y] = false;
    } else {
      barriers.vertical[move.barrier_position_x][move.barrier_position_y] = false;
    }
    if (move.player_isWhite)
      players[0].barriers_left++;
    else
      players[1].barriers_left++;
  } else { // Else player movement
    if (move.player_isWhite) {
      players[0].position_x = move.player_originalPosition_x;
      players[0].position_y = move.player_originalPosition_y;
    } else {
      players[1].position_x = move.player_originalPosition_x;
      players[1].position_y = move.player_originalPosition_y;
    }
  }
}

void QuoridorBoard::debug_printMove(Move move, uint8_t indent) {
  for (size_t i = 0; i < indent; ++i) cout << "  ";
  if (move.isBarrierPlacement) {
    if (move.barrier_isHorizontal) {
      cout << "barrier(horizontal," << +move.barrier_position_x << "," << +move.barrier_position_y << ")" << endl;
    } else {
      cout << "barrier(vertical," << +move.barrier_position_x << "," << +move.barrier_position_y << ")" << endl;
    }
  } else { // Else player movement
    if (move.player_isWhite) {
      cout << "move(white," << +move.player_movePosition_x << "," << +move.player_movePosition_y << ")" << endl;
    } else {
      cout << "move(black," << +move.player_movePosition_x << "," << +move.player_movePosition_y << ")" << endl;
    }
  }
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
        if (x != 7 && barriers.horizontal[x + 1][y])
          std::cout << "ERROR: Invalid board state, overlapping horizontal barriers at positions " << x << "-" << y << " and " << x + 1 << "-" << y << endl;
      } else if (barriers.vertical[x][y]) {
        if (y != 7 && barriers.vertical[x][y + 1])
          std::cout << "ERROR: Invalid board state, overlapping vertical barriers at positions " << x << "-" << y << " and " << x << "-" << y + 1 << endl;
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

  // Grid of legal moves
  BarrierGrid legalBarrierPlacemenent;

  // Random number generators
  random_device randseed;
  mt19937 randgen{ randseed() };
  uniform_int_distribution<> rand_0_20(0, 20);
  uniform_int_distribution<> rand_0_7(0, 7);
  uniform_int_distribution<> rand_0_1(0, 1);

  // Random number of barrier to try to add
  uint8_t number_barriers_trial = rand_0_20(randgen);
  uint8_t number_barriers_real = 0;

  // Place barriers at random positions, if inval then drop (no retry)
  for (int barrier = 0; barrier < number_barriers_trial; ++barrier) {
    // Update legal moves after each barrier placed
    legalBarrierPlacemenent = get_legalBarrierPlacements();
    // Random orientation, true=horizontal false=vertical
    bool orientation = rand_0_1(randgen);
    // Random position
    uint8_t x = rand_0_7(randgen);
    uint8_t y = rand_0_7(randgen);
    // Check if legal and add the barrier
    if (orientation && legalBarrierPlacemenent.horizontal[x][y]) {
      barriers.horizontal[x][y] = true;
      number_barriers_real++;
    }
    else if (legalBarrierPlacemenent.vertical[x][y]) {
      barriers.vertical[x][y] = true;
      number_barriers_real++;
    }
  }

  // Randomly discount the placed barriers from either players
  uniform_int_distribution<> rand_placed_by_white( number_barriers_real > 10 ? number_barriers_real - 10 : 0 , number_barriers_real > 10 ? 10 : number_barriers_real);
  uint8_t rand_number_barrier_placed_by_white = rand_placed_by_white(randgen);
  uint8_t rand_number_barrier_placed_by_black = number_barriers_real - rand_number_barrier_placed_by_white;
  players[0].barriers_left = 10 - rand_number_barrier_placed_by_white;
  players[1].barriers_left = 10 - rand_number_barrier_placed_by_black;
}

void QuoridorBoard::debug_setRandomPlayerPositions() {
  std::cout << "DEBUG: Placing the players at random positions." << endl;

  random_device randseed;
  mt19937 randgen{ randseed() };
  uniform_int_distribution<> rand_0_8(0, 8);

  // Place players at random positions without checking for supperposition
  players[0].position_x = rand_0_8(randgen);
  players[0].position_y = rand_0_8(randgen);
  players[1].position_x = rand_0_8(randgen);
  players[1].position_y = rand_0_8(randgen);
}
