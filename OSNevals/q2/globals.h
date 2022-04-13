#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include "spec.h"

extern sem_t H, A, N;
extern spec **SPECTATORS;
extern sem_t **spec_locks;
extern sem_t **spec_wait;

extern pthread_cond_t **match_wait;
extern pthread_mutex_t **spectator;

extern int g;    //number of goal events
extern int X, G; // spectating timer, number of groups
extern pthread_cond_t **group_wait;

#define BOLD "\033[1m"
#define NO_BOLD "\033[22m"
#define RED "\033[38;5;1m"
#define GREEN "\033[38;5;121m"
#define ORANGE "\033[38;5;209m"
#define PURPLE "\033[38;5;205m"
#define LIGHT_PINK "\033[38;5;225m"
#define LIGHT_PURPLE "\033[38;5;213m"
#define YELLOW "\033[38;5;223m"
#define RESET "\033[0m"
#define BLUE "\033[0;34m"

extern int *groups;

typedef struct goals goals;

struct goals
{
    float prob;
    char team;
    int time;
};

void *init_timer(void *arg);
void *init_spectators(void *arg);
extern goals *GOALS;
extern time_t clock_time;

#endif