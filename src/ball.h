#ifndef BALL_H
#define BALL_H

#include "court.h"
#include "board.h"
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define X_DELAY 5
#define Y_DELAY 8

struct Ball {
  int dx;        // directional shift wrt x
  int dy;        // directional shift wrt y
  int x;         // current x
  int y;         // current y
  int speed;     // speed of ball
  int x_delay;   // period, in ms, before ball moves along x
  int x_elapsed; // prior refresh on x
  int y_elapsed; // prior refresh on y
  int y_delay;   // period, in ms before ball moves along y
};

void serve_ball(WINDOW *window, struct Ball *ball);
void move_ball(WINDOW *window, struct Board *board, struct Ball *ball, struct Paddle *paddle, struct Court *court);

#endif