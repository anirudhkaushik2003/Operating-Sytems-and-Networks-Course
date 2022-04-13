#ifndef COURSE_H
#define COURSE_H

#include "globals.h"
#include <pthread.h>
#include "lab.h"




typedef struct course course;
typedef struct lab lab;


enum course_state
{
    waiting_TA,
    waiting_slots,
    conducting_tut,
    withdrawn
};
struct course
{
    int id;
    char *name;
    float interest;
    int num_labs;
    int *Labs;
    int course_max_slot;
    enum course_state state;
};



void *init_course(void *arg);



#endif