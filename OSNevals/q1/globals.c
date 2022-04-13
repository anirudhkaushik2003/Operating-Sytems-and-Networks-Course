#include "globals.h"
lab *LABS;
student *STUDENTS;
course *COURSES;

pthread_mutex_t get_TA_lock[MAX_COURSES][MAX_TAS];
pthread_cond_t get_TA[MAX_COURSES][MAX_TAS];

pthread_mutex_t lab_lock[MAX_LABS];

pthread_mutex_t student_lock[MAX_STUDENTS];
pthread_cond_t tutorial[MAX_COURSES][MAX_STUDENTS];

int times;
void init_lab_locks()
{
    for (int i = 0; i < num_labs; i++)
    {
        int rc = pthread_mutex_init(&lab_lock[i], NULL);
        assert(rc == 0);
    }
}

void init_TA_locks()
{
    for (int i = 0; i < num_labs; i++)
    {

        for (int j = 0; j < LABS[i].num_TA; j++)
        {
            int rc = pthread_mutex_init(&get_TA_lock[i][j], NULL);
            assert(rc == 0);
            rc = pthread_cond_init(&get_TA[i][j], NULL);
            assert(rc == 0);
        }
    }
}

void init_student_locks()
{
    for (int i = 0; i < num_students; i++)
    {
        int rc = pthread_mutex_init(&student_lock[i], NULL);
        assert(rc == 0);
    }
}

void init_tutorials()
{
    for (int i = 0; i < num_courses; i++)
    {
        for (int j = 0; j < num_students; j++)
        {
            int rc = pthread_cond_init(&tutorial[i][j], NULL);
            assert(rc == 0);
        }
    }
}

void assign_TA(course *arg)
{

    int rc;

    int cond = 0;
    int i, j;
    for (i = 0; i < arg->num_labs; i++)
    {
        for (j = 0; j < LABS[arg->Labs[i]].num_TA; j++)
        {
            if (LABS[arg->Labs[i]].Tas[j].state == unassigned)
            {
                pthread_mutex_lock(&get_TA_lock[arg->Labs[i]][j]);

                if (LABS[arg->Labs[i]].Tas[j].state == unassigned && LABS[arg->Labs[i]].Tas[j].taship < LABS[arg->Labs[i]].quota && LABS[arg->Labs[i]].Tas[j].taship < LABS[arg->Labs[i]].bonus)
                {
                    LABS[arg->Labs[i]].Tas[j].taship += 1;
                    LABS[arg->Labs[i]].Tas[j].current_course = arg->id;
                    LABS[arg->Labs[i]].Tas[j].state = assigned;
                    cond = 1;
                    pthread_mutex_unlock(&get_TA_lock[arg->Labs[i]][j]);

                    break;
                }
                pthread_mutex_unlock(&get_TA_lock[arg->Labs[i]][j]);
            }
        }
        if (cond == 1)
        {
            break;
        }
    }
    if (cond == 1)
    {
        printf(BLUE "TA %d from lab %s has been allocated to course %s for the %d TA ship\n" RESET, LABS[arg->Labs[i]].Tas[j].id, LABS[arg->Labs[i]].name, arg->name, LABS[arg->Labs[i]].Tas[j].taship);

        arg->state = waiting_slots;
        sleep(1);
        while (arg->state == waiting_slots)
        {

            conduct_tutorial(arg->Labs[i], j);
        }
    }
}

int check_labs(course *c)
{
    for (int i = 0; i < c->num_labs; i++)
    {
        if (LABS[c->Labs[i]].state == TA_available)
        {
            if (check_Tas(c->Labs[i]) == 1)
            {
                return 1;
            }
        }
    }
    for (int i = 0; i < c->num_labs; i++)
    {
        if (LABS[c->Labs[i]].state == TA_available)
        {
            return 0;
        }
    }
    return -1;
}
int check_Tas(int index)
{

    for (int j = 0; j < LABS[index].num_TA; j++)
    {
        if (LABS[index].Tas[j].state == unassigned)
        {
            return 1;
        }
    }

    return -1;
}

int check_students()
{
    for (int i = 0; i < num_students; i++)
    {
        if (STUDENTS[i].state != exited_simulation && STUDENTS[i].state != selected_course)
        {
            return 1;
        }
    }
    return -1;
}

void conduct_tutorial(int lab_id, int ta_id)
{
    pthread_mutex_lock(&get_TA_lock[lab_id][ta_id]);

    srand(time(NULL));
    int D = (rand() % COURSES[LABS[lab_id].Tas[ta_id].current_course].course_max_slot) + 1;
    int W = 0;

    for (int i = 0; i < num_students; i++)
    {
        if ((STUDENTS[i].pref[STUDENTS[i].current_pref] == LABS[lab_id].Tas[ta_id].current_course) && (STUDENTS[i].state == waiting))
        {
            W++;
        }
    }
    sleep(1); //wait 1 time unit

    pid_t x = syscall(__NR_gettid);
    if (D > W)
    {
        D = W;
    }

    int i, j;
    for (i = 0, j = 0; j < D; i++)
    {
        if ((STUDENTS[i].pref[STUDENTS[i].current_pref] == LABS[lab_id].Tas[ta_id].current_course) && (STUDENTS[i].state == waiting))
        {
            pthread_cond_signal(&tutorial[LABS[lab_id].Tas[ta_id].current_course][i]);
            pthread_mutex_lock(&student_lock[i]);

            STUDENTS[i].state = attending_tut;
            printf(PURPLE "Student %d has been allocated a seat in course %s\n" RESET, STUDENTS[i].id, COURSES[LABS[lab_id].Tas[ta_id].current_course].name);
            pthread_mutex_unlock(&student_lock[i]);

            j++;
        }
    }

    printf(YELLOW "Course %s has been allocated %d seats\n" RESET, COURSES[LABS[lab_id].Tas[ta_id].current_course].name, D);
    printf(GREEN "Tutorial has started for course %s with %d seats filled out of %d\n" RESET, COURSES[LABS[lab_id].Tas[ta_id].current_course].name, D, COURSES[LABS[lab_id].Tas[ta_id].current_course].course_max_slot);
    LABS[lab_id].Tas[ta_id].state = in_tut;
    pthread_mutex_unlock(&get_TA_lock[lab_id][ta_id]);
    sleep(2);

    pthread_mutex_lock(&get_TA_lock[lab_id][ta_id]);
    for (int i = 0, j = 0; j < D; i++)
    {
        if (STUDENTS[i].pref[STUDENTS[i].current_pref] == LABS[lab_id].Tas[ta_id].current_course && STUDENTS[i].state == attending_tut)
        {
            pthread_cond_signal(&tutorial[LABS[lab_id].Tas[ta_id].current_course][i]);
            pthread_mutex_lock(&student_lock[i]);
            srand(time(NULL));
            float selection = (STUDENTS[i].calibre * COURSES[LABS[lab_id].Tas[ta_id].current_course].interest);
            if (((float)rand() / (float)RAND_MAX) < selection)
            {
                STUDENTS[i].state = selected_course;
                printf(GREEN "Student %d has select the course %s permanently\n" RESET, STUDENTS[i].id, COURSES[LABS[lab_id].Tas[ta_id].current_course].name);
            }
            else
            {
                printf(RED "Student %d has withdrawn from the course %s\n" RESET, STUDENTS[i].id, COURSES[LABS[lab_id].Tas[ta_id].current_course].name);

                if (STUDENTS[i].current_pref < 2)
                {
                    STUDENTS[i].current_pref += 1;
                    STUDENTS[i].state = waiting;
                    printf(BLUE "Student %d has changed preference from the course %s(%d) to course %s(%d)\n" RESET, STUDENTS[i].id, COURSES[STUDENTS[i].pref[STUDENTS[i].current_pref - 1]].name, STUDENTS[i].current_pref - 1, COURSES[STUDENTS[i].pref[STUDENTS[i].current_pref]].name, STUDENTS[i].current_pref);
                }
                else
                {
                    printf(RED "Student %d couldn't get any of his preferred courses\n" RESET, STUDENTS[i].id);
                    STUDENTS[i].state = exited_simulation;
                }
            }
            pthread_mutex_unlock(&student_lock[i]);

            j++;
        }
    }
    COURSES[LABS[lab_id].Tas[ta_id].current_course].state = waiting_TA;

    printf(RED "TA %d from lab %s has completed the tutorial and left the course %s\n" RESET, ta_id, LABS[lab_id].name, COURSES[LABS[lab_id].Tas[ta_id].current_course].name);

    LABS[lab_id].Tas[ta_id].state = unassigned;
    LABS[lab_id].Tas[ta_id].current_course = -1;

    pthread_mutex_unlock(&get_TA_lock[lab_id][ta_id]);
}
