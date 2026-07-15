#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20

typedef struct {
    int cells[BOARD_HEIGHT][BOARD_WIDTH];
} Board;

void init_board(Board *b);

bool is_cell_occupied(Board *b, int x, int y);

void occupy_cell(Board *b, int x, int y);

#endif 