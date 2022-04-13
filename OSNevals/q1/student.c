#include "student.h"



void fill_registration(student *arg)
{
    sleep(arg->time);
    printf(RED "Student %d has filled in preferences for course registration" RESET "\n", arg->id);
    arg->state = waiting;
}

void *init_student(void *arg)
{
    student *s = (student *)arg;
    //printf("%d %f %d %d %d %d\n", s->id, s->calibre, s->pref1, s->pref2, s->pref3, s->time);
    while (1)
    {
        while (s->state == not_registered)
        {
            pthread_mutex_lock(&student_lock[s->id]);
            fill_registration(s);
            pthread_mutex_unlock(&student_lock[s->id]);
        }
        if (s->state == waiting)
        {
            if (COURSES[s->pref[s->current_pref]].state == withdrawn)
            {
                pthread_mutex_lock(&student_lock[s->id]);
                if (STUDENTS[s->id].current_pref < 2)
                {
                    STUDENTS[s->id].current_pref += 1;
                    printf( "Student %d has changed preference from the course %s(%d) to course %s(%d)\n" RESET, STUDENTS[s->id].id, COURSES[STUDENTS[s->id].pref[STUDENTS[s->id].current_pref - 1]].name, STUDENTS[s->id].current_pref - 1, COURSES[STUDENTS[s->id].pref[STUDENTS[s->id].current_pref]].name, STUDENTS[s->id].current_pref);
                }
                else
                {
                    printf("Student %d couldn't get any of his preferred courses\n" RESET, STUDENTS[s->id].id);
                    STUDENTS[s->id].state = exited_simulation;
                }
                pthread_mutex_unlock(&student_lock[s->id]);
            }
            else
            {
                pthread_mutex_lock(&student_lock[s->id]);
                pthread_cond_wait(&tutorial[s->pref[s->current_pref]][s->id], &student_lock[s->id]);
                pthread_mutex_unlock(&student_lock[s->id]);
            }
        }
        while (s->state == attending_tut)
        {
        }
        if (s->state == exited_simulation || s->state == selected_course)
            pthread_exit(NULL);
    }
}
