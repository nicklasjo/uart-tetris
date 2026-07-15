#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "board.h"
#include "piece.h"

int find_full_lines(Board *b);
void clear_full_lines(Board *b, int y);

#endif