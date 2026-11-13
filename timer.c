#include "timer.h"

#include <sys/time.h> // handling of timer
#include <signal.h>   // for sig_atomic_t
#include <stddef.h>   // so we have null

volatile sig_atomic_t ticked = 0;

/* handle_timer -- handles board if resized
 *   args:
 *     a) sig: what signal
 *   output: none
 *   errs: none
 *   returns none
 */
void handle_timer(int sig) {
  (void)sig; // required, not used
  ticked++; // increments
}

/* clear_timer -- clears timer that was raising signal
 *   args: none
 *   output: none
 *   errs: none
 *   returns none
 */
void clear_timer() {
  struct itimerval cleared = {0};         // builds cleared timer
  setitimer(ITIMER_REAL, &cleared, NULL); // binds timer with ITIMER_REAL
}