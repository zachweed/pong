#include "ball.h"
#include "board.h"
#include "paddle.h"
#include "court.h"
#include "timer.h"

#include <stdlib.h>   // standard library
#include <string.h>   // string stuff
#include <ncurses.h>  // terminal interactions
#include <signal.h>   // signals
#include <sys/time.h> // timers

volatile sig_atomic_t resized; // window resized

#define TIMER_PERIOD_S   0
#define TIMER_PERIOD_MIC 50000
#define RANDOM_SEED_A    64
#define RANDOM_SEED_B    63
#define DEFAULT_DELAY    50

/* handle_resize -- handles board if resized
 *   args:
 *     a) sig: what signal
 *   output: none
 *   errs: none
 *   returns none
 */
void handle_resize(int sig) {
	(void)sig; // required, not used
	resized = 1;
}

/* initialize_ball -- builds ball and board
 *   args:
 *     a) *ball: pointer on ball
 *     b) *board: pointer on board
 *   output: none
 *   errs: none
 *   returns none
 */
void initialize_ball(struct Ball *ball, struct Board *board) {
	board->in_progress = 1;                                 // indicates in progress;
	ball->x_delay = DEFAULT_DELAY + rand() % RANDOM_SEED_B; // X speed
	ball->y_delay = DEFAULT_DELAY + rand() % RANDOM_SEED_A; // Y speed
	board->last_speed_increase_at = time(NULL);             // speed increase
	ball->y_elapsed = 0;                                    // since update on x
	ball->x_elapsed = 0;                                    // since update on y
}
	
/* initialize_setup -- intializes setup
 *   args:
 *     a) *window: pointer on window
 *     b) *board: pointer on board
 *     c) *ball: pointer on ball
 *     d) *court: pointer on court
 *   output: none
 *   errs: none
 *   returns none
 */
void initialize_setup(WINDOW *window, struct Board *board, struct Ball *ball, struct Court *court) {
	update_started_at(board);               // indicates things started
	draw_board(window, board);              // draws board
	initialize_court(window, court);        // initializes court
	draw_court(window, board);              // draws court
	draw_left_right_borders(window, board); // handles borders in court
}

/* initialize_timer -- initializes a timer
 *   args: 
 *     a) *blocked: a pointer on what is being blocked
 *   output: none
 *   errs: none
 *   returns none
 */
void initialize_timer(sigset_t *blocked) {
	struct itimerval timer;                       // timer
	timer.it_value.tv_sec = TIMER_PERIOD_S;       // initializes seconds
	timer.it_value.tv_usec = TIMER_PERIOD_MIC;    // initializes microseconds
	timer.it_interval.tv_sec = TIMER_PERIOD_S;    // interval of timer
	timer.it_interval.tv_usec = TIMER_PERIOD_MIC; // interval of timer
	setitimer(ITIMER_REAL, &timer, NULL);         // builds timer

	// provides a way of temporarily
	// blocking SIGALARM, while ticked
	// variable is being modified
	// stores empty signal flags in a set of block
	sigemptyset(blocked);
	// indicates the signal to work with is SIGALRM
	sigaddset(blocked, SIGALRM);
}

/* initialize_screen -- initializes for window and stuff
 *   args: none
 *   output: none
 *   errs: none
 *   returns none
 */
void initialize_screen() {
	initscr(); // "starts curses mode"
	cbreak(); // essentially non-canonical mode
	noecho(); // removes keypresses from screen
	curs_set(0);
}

/* clear_terminal -- clears window and frees memory
 *   args: none
 *   output: none
 *   errs: none
 *   returns none
 */
void clear_terminal(WINDOW *window) {
	wrefresh(window); // send screen refresh
	delwin(window); // destroys window & deallocates memory
	endwin();
}

int main(int argc, char *argv[]) {
	sigset_t blocked, backup;                           // a set of signals to block
	int min_speed = 18;                                 // threshold speed
	int height = 3;                                     // centering
	int width = 10;                                     // also
	srand(time(NULL));                                  // random number generator
	struct sigaction sa;                                // signal event handler
	struct Board board;                                 // initializes board
	struct Ball ball;                                   // initializes ball
	struct Court court;                                 // initializes court
	initialize_screen();                                // builds screen
	initialize_board(height, width, &board);                           // builds board
	refresh();                                                         // renders things  
	WINDOW *window = newwin(height, width, board.height, board.width); // imaginary window
	if (window == NULL) {
		perror("window not allocated");
		return 1;
	}
	initialize_setup(window, &board, &ball, &court);    // initializes structures
	board.score = 0;                                    // starts with score
	memset(&sa, 0, sizeof(sa));                         // allocates memory with non-junk data
	sa.sa_handler = &handle_timer;                      // sets signal handler
	sigaction(SIGALRM, &sa, NULL);                      // attaches on SIGALRM
	struct Paddle *paddle = create_paddle(window);      // intializes paddle
	if (paddle == NULL) {
		perror("error creating paddle");
		return 1;
	}
	draw_paddle(window, paddle);                        // draws paddle
	initialize_ball(&ball, &board);                     // builds ball
	serve_ball(window, &ball);                          // serves ball from center
	signal(SIGWINCH, handle_resize);                    // signal handler for TILT
	nodelay(window, TRUE);                              
	initialize_timer(&blocked);                         // sets timer for #ball_move()

	while (1) {
		time_t now = time(NULL); // determines how much time has passed
		double elapsed = difftime(now, board.last_speed_increase_at);
		int ch = wgetch(window);                          // keypress from window
		if (resized) {
			int row, col;
			getmaxyx(window, row, col);                     // dimensions
			flushinp();                                     // clears buffered input
			nodelay(window, FALSE);                         // allow delay of refresh
			mvwprintw(window, row / 2, col / 2, "TILT");    // print TILT
			wrefresh(window);                               // display
			clear_timer();
			wgetch(window);                                 // standby user input
			break;
		}
		if (elapsed >= 60) {
			if ( ball.x_delay - 2 >= min_speed ) {          // ball reasonable on x
				ball.x_delay -= 2;
				board.last_speed_increase_at = now;           // stores speed adjusted
			}
			if ( ball.y_delay - 2 >= min_speed ) {          // ball reasonable on y
				ball.y_delay -= 2;
				board.last_speed_increase_at = now;           // speed adjusted
			}
		}
		draw_score(window, &board);                       // draws score on board
		draw_time_elapsed(window, &board);                // draws time elapsed
		sigprocmask(SIG_BLOCK, &blocked, &backup);        // unblocks SIGALRM
		int ticked_backup = ticked;                       // state while blocked
		ticked = 0;                                       // resets ticked
		sigprocmask(SIG_SETMASK, &backup, NULL);          // restores from backup
		if ( ticked_backup > 0 ) {
			for (int i = 0; i < ticked_backup && board.in_progress; i++) {
				move_ball(window, &board, &ball, paddle, &court); // adjusts for each tick
			}
		}
		if (!board.in_progress) { break; } // break if balls out
		if (ch == 'q') { break; } // handles q
		if (ch == ',') { move_paddle_left(window, paddle); } // handles ,
		if (ch == '.') { move_paddle_right(window, paddle); } // handles .
	}
	clear_timer();          // clears timers
	clear_terminal(window); // clears visuls
	paddle_free(paddle);    // frees paddle 
	return 0;
}