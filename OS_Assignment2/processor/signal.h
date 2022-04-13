#ifndef SIGNAL_H
#define SIGNAL_H

#include <signal.h>
#include "../globals.h"

void signal_c(int sigint);
void signal_z(int sigint);
void init_signals();

#endif