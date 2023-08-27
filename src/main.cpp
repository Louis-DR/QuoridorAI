#include <iostream>
#include "QuoridorBoard.hpp"

int  main() {
    QuoridorBoard board;
    board.print();
    board.debug_setRandomPlayerPositions();
    board.print();
    board.debug_setRandomBarriers();
    board.print();
    board.debug_setRandomPlayerPositions();
    board.print();
    board.debug_setRandomBarriers();
    board.print();

    return 0;
}