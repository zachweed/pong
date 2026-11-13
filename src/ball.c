#include "ball.h"
#include "board.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define RAND_MOD 5

/* choose_random -- chooses integer value of 1/2
 *   args:
 *     a) *invar: pointer on input variable
 *   output: none
 *   errs: none
 *   returns none
 */
void choose_random(int *invar) {
	if (invar == NULL) {
		perror("null value supplied");
	} else {
		*invar = (rand() % 2);  
	}
}

/* serve_ball -- serves ball from center
 *   args:
 *     a) *window: pointer on window location
 *     b) *board: pointer on board
 *     b) *ball: pointer on ball
 *     b) *paddle: pointer on paddle
 *   output: none
 *   errs: none
 *   returns none
 */
void serve_ball(WINDOW *window, struct Ball *ball) {
	if (!window || !ball) {
		perror("missing window and/or ball");
		return;
	}
	int ovar;                                 // var storing random direction of ball
	int row, col;                             // dimensions
	getmaxyx(window, row, col);
	ball->x = col / 2;                        // draws ball at exact center with col
	ball->y = row / 2;                        // centers with row
	choose_random(&ovar);                     // chooses random direction to send ball in
	if (ovar == 1) {                          // randomly determines direction                          
		ball->dx = 1;                           // moves ball right
	} else {
		ball->dx = -1;                          // moves ball left
	}
	ball->dy = 1;                             // moves ball down
	mvwprintw(window, ball->y, ball->x, "O"); // given coordinates, draws ball from center
	wrefresh(window);                         // renders
}

/* move_ball -- moves ball
 *   args:
 *     a) *window: pointer on window location
 *     b) *board: pointer on board
 *     c) *ball: pointer on ball
 *     d) *paddle: pointer on paddle
 *     e) *court: pointer on court
 *   output: none
 *   errs: none
 *   returns none
 */
void move_ball(WINDOW *window, struct Board *board, struct Ball *ball, struct Paddle *paddle, struct Court *court) {
	if (!window || !board || !ball || !paddle || !court) {
		perror("missing arguments");
		return;
	}

	int ovar;                                 // variable determining adjustment
	int next_x = ball->x + ball->dx;          // next_x based on dx
	int next_y = ball->y + ball->dy;          // next_x based on dx
	mvwaddch(window, ball->y, ball->x, ' ');  // clears prior position
	ball->x_elapsed += 50;                    // sets to ms since last update
	ball->y_elapsed += 50;                    // sets to ms since last update
	if ( ball->x_elapsed >= ball->x_delay ) {                                    // move ball horizontally if beyond delay
		if ( next_x >= court->right_boundary || next_x <= court->left_boundary ) { // if next position at left/right wall
			ball->dx = -ball->dx;   // changes direction
		}
		ball->x += ball->dx;      // adjusts position
		ball->x_elapsed = 0;      // resets movement
	}
	
	if ( ball->y_elapsed >= ball->y_delay ) { // move ball vertically if beyond delay
		// if at paddle and moving down, shift direction & increment score
		if ( ball->x >= paddle->left_x && ball->x <= paddle->right_x && next_y >= paddle->y && ball->dy > 0 ) {
			choose_random(&ovar); // chooses random speed adjustment
			if (ovar) {
				ball->x_delay += 1 + rand() % RAND_MOD; // increments speed
				ball->y_delay += 1 + rand() % RAND_MOD;
			} else {
				ball->x_delay -= 1 + rand() % RAND_MOD; // decrements speed
				ball->y_delay -= 1 + rand() % RAND_MOD;
			}
			board->score++;            // increases score on interaction with paddle
			draw_score(window, board); // redraws score
			ball->dy = -ball->dy;      // moves ball upwards
		}
		// if beyond paddle and moving down
		if ( (next_x < paddle->left_x || next_x > paddle->right_x) && ( next_y >= paddle->y && ball->dy > 0 ) ) {
			int row, col;
			getmaxyx(window, row, col);                         // grab dimensions
			board->balls_remaining -= 1;                        // adjusts balls remaining
			if (board->balls_remaining == 0) {                  // if no balls remaining
				mvwprintw(window, row / 2, col / 2, "COMPLETE");  // indicate complete
				wrefresh(window);                                 // show string
				napms(75);                                        // pauses for a little
				board->in_progress = 0;                           // flips a flag soe caller can clear screen
				return;
			} else {                                            // if balls remaining
				draw_balls_left(window, board);                   // indicate so
				serve_ball(window, ball);                        // and re-serves ball
				return;  
			}
		}
		if ( next_y <= court->top_boundary ) { ball->dy = -ball->dy; } // if at top, shift direction (downwards)
		ball->y += ball->dy;                                           // adjusts vertical
		ball->y_elapsed = 0;                                           // resets counter on most recent movement
	}

	mvwprintw(window, ball->y, ball->x, "O"); // moves ball and renders
	wrefresh(window);
}