#include <stdint.h>
#include "uart.h"
#include "led.h"
#include "systick.h"
#include "board.h"
#include "piece.h"
#include "game.h"
#include "rng.h"
#include "score.h"

#define START_X 4
#define START_Y 0



int main(void) {
    init_usart();
    init_led();
    init_systick();

    // --- set up one static frame ---
    Board board;
    init_board(&board);


    rng_seed(millis());
    Piece piece;
    spawn_random_piece(&piece, START_X, START_Y);

    GameStats stats;
    init_stats(&stats);

    send_string("\033[2J\033[H");
    draw(&board, &piece);

    print_game_start_message();
    char start_char;
    while (1) {
        if (try_receive_char(&start_char) && start_char == '\r') {
            clear_text_display();
            break;
        }
    }
    draw_stats(&stats);

    uint32_t last_drop = millis();
    const uint32_t drop_interval = 500;  // ms per step down
    bool redraw = false;
    bool game_over = false;

    while (1) {
        char received_char;

        if (try_receive_char(&received_char)) {
            if (received_char == 'a') move_piece_laterally(&piece, &board, -1);
            else if (received_char == 'd') move_piece_laterally(&piece, &board, 1);
            else if (received_char == 's') move_piece_bottom(&piece, &board);
            else if (received_char == ' ') rotate_piece(&piece, &board);
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
            register_line_clear(&stats);    
            redraw = true;
        }

        if (redraw) {
            draw(&board, &piece);
            draw_stats(&stats);
            redraw = false;
        }

        if (piece_has_landed(&piece, &board)) {
            lock_piece(&piece, &board);
            spawn_random_piece(&piece, START_X, START_Y);
            if (!can_place_piece(&piece, &board)) {
                game_over_stats(&stats);
                break;  // Game over
            }
        }
        

    }
}