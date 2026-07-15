#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>
#include <stdbool.h>
#include "board.h"


typedef struct {
    int x;
    int y;
    int rotation;
    uint8_t shape[1][1];
} Piece;

void init_piece(Piece *p, int x, int y, uint8_t shape[1][1]);
bool can_place_piece(Piece *p, Board *b);
void draw(Board *b, Piece *p);
bool move_piece_laterally(Piece *p, Board *b, int dx);
bool move_piece_down(Piece *p, Board *b);
bool piece_has_landed(Piece *p, Board *b);

#endif