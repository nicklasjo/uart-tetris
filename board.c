#include "board.h"
#include <stdint.h>
#include <stdbool.h>

void init_board(Board *b) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            b->cells[y][x] = 0;
        }
    }
}

bool is_cell_occupied(Board *b, int x, int y) {
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        return true; // Out of bounds is considered occupied
    }

    if (b->cells[y][x] != 0) {
        return true; // Cell is occupied
    }

    else {
        return false; // Cell is free
    }
    
}

