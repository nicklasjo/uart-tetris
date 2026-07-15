#include "game.h"

int find_full_lines(Board *b) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        int count = 0;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (b->cells[y][x] != 0) count++;
        }
        if (count == BOARD_WIDTH) return y;
    }
    return -1;  // no full line found
}

void clear_full_lines(Board *b, int y) {
    for (int col = 0; col < BOARD_WIDTH; col++) {
        b->cells[y][col] = 0;
    }
}




