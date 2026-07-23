#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

typedef struct {
    uint32_t score;
    uint32_t lines_cleared;
} GameStats;

void init_stats(GameStats *s);
void register_line_clear(GameStats *s);
void draw_stats(GameStats *s);
void game_over_stats(GameStats *s);
void print_game_start_message(void);
void clear_text_display(void);

#endif
