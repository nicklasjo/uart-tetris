#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>
#include <stdbool.h>
#include "board.h"


typedef struct {
    int x;
    int y;
    int rotation;
    uint8_t shape[4][4];
} Piece;

#define NUM_TETROMINOES 7

void init_piece(Piece *p, int x, int y, const uint8_t shape[4][4]);
void spawn_random_piece(Piece *p, int x, int y);
bool can_place_piece(Piece *p, Board *b);
void draw(Board *b, Piece *p);
bool move_piece_laterally(Piece *p, Board *b, int dx);
bool move_piece_down(Piece *p, Board *b);
bool piece_has_landed(Piece *p, Board *b);
bool move_piece_bottom(Piece *p, Board *b);
bool rotate_piece(Piece *p, Board *b);
void lock_piece(Piece *p, Board *b);

#endif