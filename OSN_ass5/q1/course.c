#include "course.h"
#include <string.h>

void *init_course(void *arg)
{
    course *c = (course *)arg;
    while (1)
    {
        while (c->state == waiting_TA)
        {
            if (check_labs(c) == -1)
            {
                COURSES[c->id].state = withdrawn;
                printf(GREEN "Course %s doesn't have any TA's eligible and is removed from course offerings\n" RESET, COURSES[c->id].name);
                for (int i = 0; i < num_students; i++)
                {
                    while ((STUDENTS[i].pref[STUDENTS[i].current_pref] == c->id) && (STUDENTS[i].state == waiting))
                    {
                        pthread_cond_signal(&tutorial[c->id][i]);
                    }
                }
                break;
            }
            else if (check_labs(c) == 1)
                assign_TA(c);
            else if (check_labs(c) == 0)
            {

                //busy wait as there is no restriction on the courses for this
            }
        }

        if (c->state == withdrawn)
        {
            printf("COURSE %s exited\n", c->name);
            pthread_exit(NULL);
        }
    }
}