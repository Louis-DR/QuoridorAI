#include "QuoridorGame.hpp"

QuoridorGame::QuoridorGame() {
  // Init barrier tables to zeros
  barriers.vertical.fill({});
  barriers.horizontal.fill({});

  // Init position of both players
  players[0].position.fill({});
  players[0].position[0][4] = true;
  players[1].position.fill({});
  players[0].position[8][4] = true;

  // Init barrier counts to zero for both players
  players[0].barriers_left = 0;
  players[1].barriers_left = 0;
}

QuoridorGame::~QuoridorGame() {

}