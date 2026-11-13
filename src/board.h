#ifndef BOARD_H
#define BOARD_H

#include "paddle.h"
#include <ncurses.h>
#include <time.h>

struct Board {
  int width;                     // width of board
  int height;                    // height
  int balls_remaining;           // how many balls
  int score;                     // total score
  struct timespec started_at;    // when started
  time_t last_speed_increase_at; // most recent increase
  int row;                       // dimensions rows
  int col;                       // dimensions columns
  int in_progress;               // if still in progress
};

void draw_board(WINDOW *window, struct Board *board);
void build_box(WINDOW *window, struct Board *board);
void build_window(WINDOW *window);
void draw_score(WINDOW *window, struct Board *board);
void update_started_at(struct Board *board);
void draw_time_elapsed(WINDOW *window, struct Board *board);
void draw_court(WINDOW *window, struct Board *board);
void draw_left_right_borders(WINDOW *window, struct Board *board);
void draw_balls_left(WINDOW *window, struct Board *board);
void initialize_board(int height, int width, struct Board *board);

#endif