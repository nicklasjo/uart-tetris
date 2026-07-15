#include "piece.h"
#include "board.h"
#include "uart.h"
#include <stdint.h>


void init_piece(Piece *p, int x, int y, uint8_t shape[1][1]) {
    p->x = x;
    p->y = y;
    p->rotation = 0;
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 1; j++) {
            p->shape[i][j] = shape[i][j];
        }
    }
}

bool can_place_piece(Piece *p, Board *b) {
    for (int py = 0; py < 1; py++) {          // piece rows
        for (int px = 0; px < 1; px++) {      // piece cols
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
    if (px < 0 || px >= 1 || py < 0 || py >= 1) return 0;
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

