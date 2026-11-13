#include "court.h"
#include <ncurses.h>
#include <string.h>

#define INNER_ROW     3
#define INNER_COL     3
#define PADDLE_OFFSET 2
#define ZERO_OFFSET   1

/* initialize_court -- initializes_court
 *   args:
 *     a) *window: pointer on window location
 *     b) *court: pointer on court
 *   output: none
 *   errs: none
 *   returns none
 */
void initialize_court(WINDOW *window, struct Court *court) {
  int col, row;
  getmaxyx(window, row, col);                               // dimensions
  court->top_boundary = INNER_ROW;                          // 3 units from outer window
  court->left_boundary = INNER_COL;                         // 3 units from outer window
  court->right_boundary = (col - ZERO_OFFSET - INNER_COL);  // inverse of left_boundary with zero-based indexing
  court->bottom_boundary = (row - PADDLE_OFFSET);           // bottom boundary, could interact with paddle
}