#ifndef LAB_H
#define LAB_H

#include "globals.h"

typedef struct lab lab;
typedef struct TA ta;

enum Lab_state
{
    TA_available,
    No_TA_available,
    All_busy
};

enum TA_state
{
    assigned,
    in_tut,
    unassigned
};
struct lab
{
    char *name;
    int id;
    int num_TA;
    int quota;
    enum Lab_state state;
    ta *Tas;
    int bonus;
};

struct TA
{
    int id;
    int taship;
    int current_course;
    enum TA_state state;
};

void create_lab(lab *Lab);
void *init_lab(void *arg);

#endif