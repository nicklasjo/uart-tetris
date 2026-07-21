#include "piece.h"
#include "board.h"
#include "uart.h"
#include "rng.h"
#include <stdint.h>

static const uint8_t TETROMINOES[NUM_TETROMINOES][4][4] = {
    { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} }, // I
    { {1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} }, // O
    { {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }, // T
    { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} }, // S
    { {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} }, // Z
    { {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }, // J
    { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }, // L
};

void init_piece(Piece *p, int x, int y, const uint8_t shape[4][4]) {
    p->x = x;
    p->y = y;
    p->rotation = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            p->shape[i][j] = shape[i][j];
        }
    }
}

void spawn_random_piece(Piece *p, int x, int y) {
    int index = rng_below(NUM_TETROMINOES);
    init_piece(p, x, y, TETROMINOES[index]);
}

bool can_place_piece(Piece *p, Board *b) {
    for (int py = 0; py < 4; py++) {          // piece rows
        for (int px = 0; px < 4; px++) {      // piece cols
            if (p->shape[py][px] != 0) {
                int bx = p->x + px;           // map to board coordinate
                int by = p->y + py;
                if (is_cell_occupied(b, bx, by)) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Value of the active piece at board coord (x, y), or 0 if the piece
// doesn't cover it. Inverse of the mapping in can_place_piece:
// board coord -> piece-local coord.
static uint8_t piece_cell_at(Piece *p, int x, int y) {
    int px = x - p->x;
    int py = y - p->y;
    if (px < 0 || px >= 4 || py < 0 || py >= 4) return 0;
    return p->shape[py][px];
}

// Full-frame renderer: repaint the whole board + active piece each call.
// Home the cursor first so this frame overwrites the previous one.
void draw(Board *b, Piece *p) {
    send_string("\033[H");                          // cursor to top-left
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        send_char('|');                             // left wall
        for (int x = 0; x < BOARD_WIDTH; x++) {
            uint8_t v = b->cells[y][x];             // settled block?
            if (v == 0) v = piece_cell_at(p, x, y); // else overlay piece
            send_string(v ? "[]" : "  ");
        }
        send_string("|\r\n");                       // right wall
    }
    for (int i = 0; i < BOARD_WIDTH * 2 + 2; i++) {
        send_char('-');                             // bottom border
    }
    send_string("\r\n");
}


bool move_piece_laterally(Piece *p, Board *b, int dx) {
    Piece moved = *p;                 // copy the whole piece (shape array included)
    moved.x += dx;                    // shift the copy
    if (can_place_piece(&moved, b)) { // is the destination legal?
        *p = moved;                   // yes → accept
        return true;
    }
    return false;                     // no → original untouched, block doesn't move
}

bool move_piece_down(Piece *p, Board *b) {
    Piece moved = *p;                 // copy the whole piece (shape array included)
    moved.y += 1;                     // shift the copy down
    if (can_place_piece(&moved, b)) { // is the destination legal?
        *p = moved;                   // yes → accept
        return true;
    }
    return false;                     // no → original untouched, block doesn't move
}

bool piece_has_landed(Piece *p, Board *b) {
    Piece moved = *p;                 // copy the whole piece (shape array included)
    moved.y += 1;                     // shift the copy down
    return !can_place_piece(&moved, b); // if can't move down, it has landed
}

int find_valid_bottom_coord(Piece *p, Board *b) {
    // int *x,*y; 
    int y_coord;
    for (y_coord = p->y; y_coord < BOARD_HEIGHT; y_coord++) {
        Piece moved = *p;
        moved.y = y_coord;
        if (!can_place_piece(&moved, b)) {
            return y_coord - 1; // Return the last valid y-coordinate
        }
    }
    return BOARD_HEIGHT - 1;
}

// Move piece all the way down
bool move_piece_bottom(Piece *p, Board *b) {
    Piece moved = *p;
    int new_y = find_valid_bottom_coord(p, b);
    moved.y = new_y;
    if (can_place_piece(&moved, b)) {
        *p = moved;
        return true;
    }

    return false;
}

// 90-degree clockwise rotation of a 4x4 shape grid: new[r][c] = old[3-c][r].
// Pure index remap, no rotation-matrix multiply / trig needed.
static void rotate_shape_cw(uint8_t src[4][4], uint8_t dst[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            dst[r][c] = src[3 - c][r];
        }
    }
}

bool rotate_piece(Piece *p, Board *b) {
    Piece rotated = *p;
    rotate_shape_cw(p->shape, rotated.shape);
    if (can_place_piece(&rotated, b)) {
        *p = rotated;
        p->rotation = (p->rotation + 1) & 3;
        return true;
    }
    return false;
}

// Bake every filled cell of the piece into the board (called once it has landed).
void lock_piece(Piece *p, Board *b) {
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (p->shape[py][px] != 0) {
                occupy_cell(b, p->x + px, p->y + py);
            }
        }
    }
}


