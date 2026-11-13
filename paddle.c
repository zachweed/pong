#include "board.h"
#include "paddle.h"
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define ROW_OFFSET 2
#define PADDLE_WIDTH 16

/* create_paddle -- creates paddle on heap (manual memory)
 *   args:
 *     a) *window: pointer on window location
 *   output: none
 *   errs: none
 *   returns none
 */
struct Paddle *create_paddle(WINDOW *window) {
  int row, col;
  getmaxyx(window, row, col);                                 // dimensions for drawing paddle

  struct Paddle *paddle = malloc(sizeof(struct Paddle));      // allocates for paddle (and members) on heap
  if (paddle == NULL) {
    perror("error allocating paddle"); /* print error */
    return NULL;
  }

  paddle->y = row - ROW_OFFSET;                               // paddle vertical position lower rows
  paddle->left_x = ( (col / 2) - ( PADDLE_WIDTH / 2 ) );      // centers left side of paddle
  paddle->right_x = ( (col / 2) + ( PADDLE_WIDTH / 2 ) ) - 1; // centers right side of paddle

  return paddle;
}

/* free_paddle -- frees memory of paddle
 *   args:
 *     a) *paddle: pointer on paddle location
 *   output: none
 *   errs: none
 *   returns none
 */
void paddle_free(struct Paddle *paddle) {
  free(paddle); // clears paddle from memory
}

/* erase_paddle -- erases paddle
 *   args:
 *     a) *window: pointer on window location
 *     b) *paddle: pointer on paddle
 *   output: none
 *   errs: none
 *   returns none
 */
void erase_paddle(WINDOW *window, struct Paddle *paddle) {
  int row, col;
  (void)col;
  getmaxyx(window, row, col);                           // dimensions
  mvwhline(window, row - 2, paddle->left_x, ' ', 16);   // draws space character
}

/* draw_paddle -- draws paddle to some coordinates
 *   args:
 *     a) *window: pointer on window location
 *     b) *paddle: pointer on paddle
 *   output: none
 *   errs: none
 *   returns none
 */
void draw_paddle(WINDOW *window, struct Paddle *paddle) {
  int row, col;
  (void)col;
  getmaxyx(window, row, col);                         // dimensions
  mvwhline(window, row - 2, paddle->left_x, '=', 16); // draws paddle as ====
  wrefresh(window);                                   // renders
}

/* move_paddle_left -- moves paddle to some coordinates
 *   args:
 *     a) *window: pointer on window location
 *     b) *paddle: pointer on paddle
 *   output: none
 *   errs: none
 *   returns none
 */
void move_paddle_left(WINDOW *window, struct Paddle *paddle) {
  if (paddle->left_x - 1 > ROW_OFFSET * 2) { // if in boundaries of court
    erase_paddle(window, paddle);            // erases original
    paddle->left_x -= 1;                     // moves left to left
    paddle->right_x -= 1;                    // moves right to left
    draw_paddle(window, paddle);             // redraws
    wrefresh(window);                        // and renders
  }
}

/* move_paddle_right -- moves paddle to some coordinates
 *   args:
 *     a) *window: pointer on window location
 *     b) *paddle: pointer on paddle
 *   output: none
 *   errs: none
 *   returns none
 */
void move_paddle_right(WINDOW *window, struct Paddle *paddle) {
  int row, col;
  (void)row;                                            // required, not used
  getmaxyx(window, row, col);                           // dimensions
  if (paddle->right_x + 1 < col - 4) {                  // if in dimensions of court
    erase_paddle(window, paddle);                       // clears paddle
    paddle->left_x += 1;                                // shifts paddle
    paddle->right_x += 1;                               // to the right
    draw_paddle(window, paddle);                        // redraws
    wrefresh(window);                                   // and renderss
  }
}
