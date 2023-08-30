#include <iostream>
#include "QuoridorBoard.hpp"

int  main() {
    QuoridorBoard board;

    board.print();
    board.config.interactive_auto_print = true;
    board.config.ai_minimax_depth = 2;

    board.startInteractiveMode();

    return 0;
}