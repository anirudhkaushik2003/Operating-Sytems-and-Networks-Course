#ifndef STUDENT_H
#define STUDENT_H


#include "globals.h"

typedef struct student student;
enum student_state
{
    not_registered,
    waiting,
    attending_tut,
    selected_course,
    exited_simulation
};

struct student
{
    int id;
    int pref[3];
    int current_pref;
    float calibre;
    int time;
    enum student_state state;
};

void fill_registration(student *arg);
void *init_student(void *arg);

#endif