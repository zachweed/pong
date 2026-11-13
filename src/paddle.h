#ifndef PADDLE_H
#define PADDLE_H

#include <ncurses.h>

struct Board;
struct Paddle {
  int left_x;  // left-most x position of paddle
  int right_x; // right-most x position of paddle
  int y;       // y position of paddle
};

void move_paddle_left(WINDOW *window, struct Paddle *paddle);
void move_paddle_right(WINDOW *window, struct Paddle *paddle);
void draw_paddle(WINDOW *window, struct Paddle *paddle);
struct Paddle *create_paddle(WINDOW *window);
void paddle_free(struct Paddle *paddle);

#endif