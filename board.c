#include "board.h"
#include <ncurses.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>

#define BALLS_INITIAL   3
#define WINDOW_ORIGIN_Y 3
#define WINDOW_ORIGIN_X 3
#define STRING_MARGIN   2
#define COURT_MARGIN    3

/* initialize_board -- fills memory with board
 *   args:
 *     a) height: height of board
 *     b) width: width of board
 *     c) board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void initialize_board(int height, int width, struct Board *board) {
  board->width = (COLS - width) / 2;      // width of board
  board->height = (LINES - height) / 2;   // height of board
  board->balls_remaining = BALLS_INITIAL; // how many remaining
}

/* update_started_at -- draws time elapsed above the pseudobox
 *   args:
 *     a) *board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void update_started_at(struct Board *board) {
  clock_gettime(CLOCK_MONOTONIC, &board->started_at); // sets started at in board
}

/* draw_left_right_borders -- draws the left and right borders
 *   args:
 *     a) *window: pointer on window location
 *     b) *board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void draw_left_right_borders(WINDOW *window, struct Board *board) {
  int row, col;                                             // dimension variables
  getmaxyx(window, row, col);                               // finds maximum dimensions and adds in row, col
  for ( int i = (COURT_MARGIN + 1); i < row - 1; i++ ) {    // starting from inside box, through every row
    mvwaddch(window, i, COURT_MARGIN, ACS_VLINE);           // print vertical left border character
    mvwaddch(window, i, col - COURT_MARGIN - 1, ACS_VLINE); // and print vertical right border characters
  }
  wrefresh(window);                                         // naturally, render
}

/* draw_court -- draws the court
 *   args:
 *     a) *window: pointer on window location
 *     b) *board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void draw_court(WINDOW *window, struct Board *board) {
  int row, col;
  getmaxyx(window, row, col);                                                      // finds current dimensions
  (void) row;                                                                      // prevents warnings, since both variables are required
  mvwhline(window, COURT_MARGIN, COURT_MARGIN, ACS_HLINE, col - COURT_MARGIN * 2); // draws a horizontal line
  wrefresh(window);                                                                // renders
}

/* draw_score -- draws the score above the pseudobox
 *   args:
 *     a) *window: pointer on window location
 *     b) *board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void draw_score(WINDOW *window, struct Board *board) {
  int row, col;               // dimension variables
  char str[15];               // buffer for score
  getmaxyx(window, row, col); // finds dimensions
  (void)row;                  // unused, though required
  snprintf(str, sizeof(str), "SCORE: %d", board->score); // interpolates score
  mvwprintw(window, 2, ( col / 2 ) - 3, "%s", str);      // centers SCORE in screen
  wrefresh(window);                                      // renders
}

/* draw_time_elapsed -- draws time elapsed above the pseudobox
 *   args:
 *     a) *window: pointer on window location
 *     b) *board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void draw_time_elapsed(WINDOW *window, struct Board *board) {
  int row, col;                                                                      // dimensions
  char str[33];                                                                      // string buffer
  (void)row;                                                                         // row requires and not used
  struct timespec this_moment;                                                       // one side of time offset
  getmaxyx(window, row, col);                                                        // finding dimensions
  clock_gettime(CLOCK_MONOTONIC, &this_moment);                                      // other side of time offset
  long duration = this_moment.tv_sec - board->started_at.tv_sec;                     // calculates total offset
  snprintf(str, sizeof(str), "ELAPSED: %02ld:%02ld", duration / 60, duration % 60);  // interpolates offset
  mvwprintw(window, STRING_MARGIN, col - strlen(str) - COURT_MARGIN - 1, "%s", str); // prints offset
  wrefresh(window);
}

/* build_window -- builds the window
 *   args:
 *     a) *window: pointer on window location
 *     b) *board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void build_window(WINDOW *window) {
  int row, col;                                                            // writes max sizes into vars
  getmaxyx(stdscr, row, col);                                              // finds maximum rows and columns
  wresize(window, row - ( COURT_MARGIN * 2 ), col - ( COURT_MARGIN * 2) ); // resizes window
  mvwin(window, WINDOW_ORIGIN_Y, WINDOW_ORIGIN_X);                         // moves window to (3, 3)
}

/* draw_balls_left -- draws time elapsed above the pseudobox
 *   args:
 *     a) *window: pointer on window location
 *     b) *board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void draw_balls_left(WINDOW *window, struct Board *board) {
  mvwprintw(window, 2, COURT_MARGIN + 1, "BALLS LEFT: %d", board->balls_remaining); // prints balls remaining
  draw_time_elapsed(window, board);                                                 // prints time_elapsed
  mvprintw(LINES - COURT_MARGIN, COURT_MARGIN + 1, "press q to exit");              // indicates how to exit
}

/* draw_build_box -- draws time elapsed above the pseudobox
 *   args:
 *     a) *window: pointer on window location
 *     b) *board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void build_box(WINDOW *window, struct Board *board) {
  draw_balls_left(window, board); // indicates balls remaining
  refresh();                      // writes string to screen
  box(window, 0, 0);              // builds and renders box, from origin
  wrefresh(window);               // renders box
}

/* draw_board -- draws time elapsed above the pseudobox
 *   args:
 *     a) *window: pointer on window location
 *     b) *board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void draw_board(WINDOW *window, struct Board *board) {
  build_window(window); // builds our window
  build_box(window, board); // builds our box
}