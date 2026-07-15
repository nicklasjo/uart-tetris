#include <stdint.h>
#include "uart.h"
#include "led.h"
#include "systick.h"
#include "board.h"
#include "piece.h"
#include "game.h"

#define START_X 4
#define START_Y 0


void game_start(void) {
    send_string("Welcome to Tetris!\r\n");
    send_string("Press enter to start the game...\r\n");
    char start_char = 0;
    while (start_char != '\r') {
        blink_led(500000);  // Blink LED while waiting for enter key
        try_receive_char(&start_char);
    }
    send_string("Game started!\r\n");
}

int main(void) {
    init_usart();
    init_led();
    init_systick();

    game_start();

    // --- set up one static frame ---
    Board board;
    init_board(&board);

    uint8_t o_shape[1][1] = { {1} };
    Piece piece;
    init_piece(&piece, START_X, START_Y, o_shape);   // O-piece at columns 4-5, rows 0-1

    send_string("\033[2J\033[H");
    draw(&board, &piece);

    uint32_t last_drop = millis();
    const uint32_t drop_interval = 250;  // ms per step down
    bool redraw = false;

    while (1) {
        char received_char;

        if (try_receive_char(&received_char)) {
            if (received_char == 'a') move_piece_laterally(&piece, &board, -1);
            else if (received_char == 'd') move_piece_laterally(&piece, &board, 1);
            toggle_led();
            redraw = true;
        }

        if (millis() - last_drop >= drop_interval) {
            last_drop = millis();
            move_piece_down(&piece, &board);
            redraw = true;
        }
        
        int full_line_y = find_full_lines(&board);
        if (full_line_y != -1) {
            clear_full_lines(&board, full_line_y);
            redraw = true;
        }

        if (redraw) {
            draw(&board, &piece);
            redraw = false;
        }

        if (piece_has_landed(&piece, &board)) {
            occupy_cell(&board, piece.x, piece.y);
            piece.y = START_Y;
            piece.x = START_X;
        }

    }
}