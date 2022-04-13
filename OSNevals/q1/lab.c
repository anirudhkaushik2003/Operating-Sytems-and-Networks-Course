#include "lab.h"

void create_lab(lab *Lab)
{
    Lab->Tas = malloc(sizeof(ta) * Lab->num_TA);
    Lab->state = TA_available;
    Lab->bonus = 1;
    for (int i = 0; i < Lab->num_TA; i++)
    {
        Lab->Tas[i].id = i;
        Lab->Tas[i].taship = 0;
        Lab->Tas[i].state = unassigned;
        Lab->Tas[i].current_course = -1;
    }
}

int check_busy(ta *t)
{
    if (t->state != unassigned)
        return 1;
    else
        return -1;
}
int check_quota(ta *t, int quota)
{
    if (t->taship < quota)
        return 1;
    else if (t->state == unassigned)
        return -1;
    return 1;
}

void *init_lab(void *arg)
{
    lab *l = (lab *)arg;
    int busy, quota;
    while (1)
    {
        busy = 0;
        quota = 0;
        for (int i = 0; i < l->num_TA; i++)
        {
            if (check_busy(&l->Tas[i]) == 1)
            {
                busy = 1;
            }
            else
            {
                busy = 0;
                break;
            }
        }
        if (busy == 1)
        {
        }
        else
        {
            pthread_mutex_lock(&lab_lock[l->id]);
            LABS[l->id].state = TA_available;
            pthread_mutex_unlock(&lab_lock[l->id]);
            for (int i = 0; i < l->num_TA; i++)
            {
                if (check_quota(&l->Tas[i], l->quota) == -1)
                {
                    quota = 1;
                }
                else
                {
                    quota = 0;
                    break;
                }
            }
            if (quota == 1)
            {

                pthread_mutex_lock(&lab_lock[l->id]);
                LABS[l->id].state = No_TA_available;
                pthread_mutex_unlock(&lab_lock[l->id]);
                printf(BOLD LIGHT_PINK "LAB %s no longer has students available for TAship\n" RESET, l->name);

                pthread_exit(NULL);
            }
        }
        int cond = 0;
        for (int i = 0; i < l->num_TA; i++)
        {
            if (LABS[l->id].Tas[i].taship == LABS[l->id].bonus)
            {
                cond = 1;
            }
            else
            {
                cond = 0;
                break;
            }
        }
        if (cond == 1 && LABS[l->id].bonus < LABS[l->id].quota)
        {
            pthread_mutex_lock(&lab_lock[l->id]);
            LABS[l->id].bonus += 1;
            pthread_mutex_unlock(&lab_lock[l->id]);
        }
    }
}