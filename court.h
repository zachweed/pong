#ifndef COURT_H
#define COURT_H

#include <ncurses.h>

struct Court {
  int top_boundary;     // top
  int left_boundary;    // left side
  int right_boundary;   // right side
  int bottom_boundary;  // bottom
};

void initialize_court(WINDOW *window, struct Court *court);

#endif