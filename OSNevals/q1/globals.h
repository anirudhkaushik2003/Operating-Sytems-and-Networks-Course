#ifndef GLOBALS_H
#define GLOBALS_H

#include "student.h"
#include "lab.h"
#include "course.h"

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>

#define MAX_TAS 20
#define MAX_COURSES 20
#define MAX_LABS 20
#define MAX_STUDENTS 100

typedef struct course_args course_args;
typedef struct course course;
typedef struct student student;
typedef struct lab lab;
typedef struct TA ta;

extern lab *LABS;
extern course *COURSES;
extern student *STUDENTS;

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

void init_lab_locks();
void init_TA_locks();
void init_student_locks();
void init_tutorials();
void assign_TA(course *arg);

int check_labs(course *c);
int check_Tas(int index);


int check_students();

void conduct_tutorial(int lab_id, int ta_id);

extern int num_courses;
extern int num_students;
extern int num_labs;

extern int times;


extern pthread_mutex_t pref_lock[MAX_COURSES];

extern pthread_mutex_t get_TA_lock[MAX_COURSES][MAX_TAS];
extern pthread_cond_t get_TA[MAX_COURSES][MAX_TAS];

extern pthread_mutex_t student_lock[MAX_STUDENTS];
extern pthread_mutex_t lab_lock[MAX_LABS];

extern pthread_cond_t tutorial[MAX_COURSES][MAX_STUDENTS];



#endif