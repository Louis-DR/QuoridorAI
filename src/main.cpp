#include <iostream>
#include "QuoridorBoard.hpp"

int  main() {
    QuoridorBoard board;
    board.print();
    board.debug_setCornerHorizontalBarriers();
    board.print();
    board.debug_setCornerVerticalBarriers();
    board.print();
    board.debug_setRandomPlayerPositions();
    board.debug_setRandomBarriers();
    board.print();
    board.debug_setRandomPlayerPositions();
    board.debug_setRandomBarriers();
    board.print();

    return 0;
}