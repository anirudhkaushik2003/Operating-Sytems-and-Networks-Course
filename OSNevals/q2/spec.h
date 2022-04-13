#ifndef SPEC_H
#define SPEC_H

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

sem_t state[20];

typedef struct spec spec;
enum spec_state
{
    not_reached,
    waiting_seat,
    watching_match,
    reached_gate,
    waiting_friends,
    exited
    
};

struct spec
{
    char *name;
    int zone;
    int P;
    int ctime; //creation time/arrival time
    int atime; //seat allocation time
    int etime; //exit time
    int group;
    enum spec_state state;
    int R;
    int id;
    int enraged;
};

#endif