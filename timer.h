#ifndef TIMER_H
#define TIMER_H

#include <signal.h>           // for sig_atomic_t

extern volatile sig_atomic_t ticked; // atomic value indicating something ticked

void handle_timer(int sig);
void clear_timer();

#endif