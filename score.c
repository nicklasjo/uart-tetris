#include "score.h"
#include "uart.h"
#include "board.h"

// Board is "|" + 2 chars/cell + "|" wide; leave a gap then start the panel.
#define STATUS_COL (BOARD_WIDTH * 2 + 5)
#define POINTS_PER_LINE 100

void init_stats(GameStats *s) {
    s->score = 0;
    s->lines_cleared = 0;
}

void register_line_clear(GameStats *s) {
    s->lines_cleared++;
    s->score += POINTS_PER_LINE;
}

void draw_stats(GameStats *s) {
    move_cursor(2, STATUS_COL);
    send_string("SCORE: ");
    send_uint(s->score);
    send_string("   "); // pad over any leftover digits from a longer previous number

    move_cursor(4, STATUS_COL);
    send_string("LINES: ");
    send_uint(s->lines_cleared);
    send_string("   ");
    move_cursor(5, STATUS_COL);
    send_string("---------------------------------");
}

void game_over_stats(GameStats *s) {
    move_cursor(2, STATUS_COL);
    send_string("GAME OVER");
    send_string("          "); // clear any leftover text
    move_cursor(4, STATUS_COL);
    send_string("FINAL SCORE: ");
    send_uint(s->score);
    send_string("   ");
    move_cursor(5, STATUS_COL);
    send_string("---------------------------------");
}

void print_game_start_message(void) {
    move_cursor(1, STATUS_COL);
    send_string("---------------------------------");
    move_cursor(2, STATUS_COL);
    send_string("Welcome to Tetris!");
    move_cursor(4, STATUS_COL);
    send_string("Press enter to start the game...");
    move_cursor(5, STATUS_COL);
    send_string("---------------------------------");
    move_cursor(6, STATUS_COL);
    send_string("Press 'a' to move left");
    move_cursor(7, STATUS_COL);
    send_string("Press 'd' to move right");
    move_cursor(8, STATUS_COL);
    send_string("Press 's' to drop down");
    move_cursor(9, STATUS_COL);
    send_string("Press space to rotate");
    move_cursor(10, STATUS_COL);
    send_string("---------------------------------");
}

void clear_text_display(void) {
    for (int row = 2; row <= 11; row++) {
        move_cursor(row, STATUS_COL);
        send_string("                                     "); // clear the line
    }
}
