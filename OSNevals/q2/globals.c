#include "globals.h"

sem_t H, A, N;
spec **SPECTATORS;
int *groups;
goals *GOALS;

time_t clock_time;
sem_t **spec_locks;
sem_t **spec_wait;

pthread_cond_t **group_wait;

int g; //number of goal events
int X, G;

void *enterzoneH(void *arg)
{
    spec *s = (spec *)arg;

    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        perror("error");
        exit(0);
    }
    ts.tv_sec += (s->ctime - clock_time + s->P);
    int result = sem_timedwait(&H, &ts);
    int res = sem_trywait(&spec_locks[s->group][s->id]);
    if (res == 0 && result == 0)
    {
        if (s->state == waiting_seat)
        {
            pthread_mutex_lock(&spectator[s->group][s->id]);
            SPECTATORS[s->group][s->id].state = watching_match;
            SPECTATORS[s->group][s->id].atime = clock_time;
            SPECTATORS[s->group][s->id].etime = SPECTATORS[s->group][s->id].atime + X;
            printf(BOLD BLUE "t=%ld: %s has got  a seat in zone H\n" RESET, clock_time, s->name);
            pthread_cond_wait(&match_wait[s->group][s->id], &spectator[s->group][s->id]);
            pthread_mutex_unlock(&spectator[s->group][s->id]);
        }

        if (SPECTATORS[s->group][s->id].state == watching_match)
        {
            pthread_mutex_lock(&spectator[s->group][s->id]);
            SPECTATORS[s->group][s->id].state = reached_gate;
            if (SPECTATORS[s->group][s->id].enraged == 1)
            {
                printf(BOLD RED "t=%ld: %s is leaving due to bad defensive performance of his team\n" RESET, clock_time, SPECTATORS[s->group][s->id].name);
            }
            else
                printf(BOLD RED "t=%ld: %s watched the match for %d seconds and is leaving\n" RESET, clock_time, SPECTATORS[s->group][s->id].name, X);
            pthread_mutex_unlock(&spectator[s->group][s->id]);
        }
    }

    if (result == 0)
    {
        /*int value;
        sem_getvalue(&H, &value);
        printf("%s before H = %d ", s->name, value);*/
        sem_post(&H);
        //sem_getvalue(&H, &value);
        //printf("%s after H = %d ", s->name, value);
    }

    pthread_exit(NULL);
}
void *enterzoneA(void *arg)
{
    

    spec *s = (spec *)arg;
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        perror("error");
        exit(0);
    }
    ts.tv_sec += (s->ctime - clock_time + s->P);
    int result = sem_timedwait(&A, &ts);
    int res = sem_trywait(&spec_locks[s->group][s->id]);
    if (res == 0 && result == 0)
    {
        if (s->state == waiting_seat)
        {
            pthread_mutex_lock(&spectator[s->group][s->id]);
            SPECTATORS[s->group][s->id].state = watching_match;
            SPECTATORS[s->group][s->id].atime = clock_time;
            SPECTATORS[s->group][s->id].etime = SPECTATORS[s->group][s->id].atime + X;
            printf(BOLD BLUE "t=%ld: %s has got  a seat in zone A\n" RESET, clock_time, s->name);
            pthread_cond_wait(&match_wait[s->group][s->id], &spectator[s->group][s->id]);
            pthread_mutex_unlock(&spectator[s->group][s->id]);
        }

        if (SPECTATORS[s->group][s->id].state == watching_match)
        {

            pthread_mutex_lock(&spectator[s->group][s->id]);
            SPECTATORS[s->group][s->id].state = reached_gate;
            if (SPECTATORS[s->group][s->id].enraged == 1)
            {
                printf(BOLD RED "t=%ld: %s is leaving due to bad defensive performance of his team\n" RESET, clock_time, SPECTATORS[s->group][s->id].name);
            }
            else
                printf(BOLD RED "t=%ld: %s watched the match for %d seconds and is leaving\n" RESET, clock_time, SPECTATORS[s->group][s->id].name, X);
            pthread_mutex_unlock(&spectator[s->group][s->id]);
        }
    }

    if (result == 0)
    {
        /*int value;
        sem_getvalue(&A, &value);
        printf("%s before A = %d ", s->name, value);*/
        sem_post(&A);
        //sem_getvalue(&A, &value);
        //printf("%s after A = %d ", s->name, value);
    }

    pthread_exit(NULL);
}
void *enterzoneN(void *arg)
{
    spec *s = (spec *)arg;

    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        perror("error");
        exit(0);
    }
    ts.tv_sec += (s->ctime - clock_time + s->P);
    int result = sem_timedwait(&N, &ts);
    int res = sem_trywait(&spec_locks[s->group][s->id]);
    if (res == 0 && result == 0)
    {
        if (s->state == waiting_seat)
        {
            pthread_mutex_lock(&spectator[s->group][s->id]);
            SPECTATORS[s->group][s->id].state = watching_match;
            SPECTATORS[s->group][s->id].atime = clock_time;
            SPECTATORS[s->group][s->id].etime = SPECTATORS[s->group][s->id].atime + X;
            printf(BOLD BLUE "t=%ld: %s has got  a seat in zone N\n" RESET, clock_time, s->name);
            pthread_cond_wait(&match_wait[s->group][s->id], &spectator[s->group][s->id]);
            pthread_mutex_unlock(&spectator[s->group][s->id]);
        }

        if (SPECTATORS[s->group][s->id].state == watching_match)
        {
            pthread_mutex_lock(&spectator[s->group][s->id]);
            SPECTATORS[s->group][s->id].state = reached_gate;
            if (SPECTATORS[s->group][s->id].enraged == 1)
            {
                printf(BOLD RED "t=%ld: %s is leaving due to bad defensive performance of his team\n" RESET, clock_time, SPECTATORS[s->group][s->id].name);
            }
            else
                printf(BOLD RED "t=%ld: %s watched the match for %d seconds and is leaving\n" RESET, clock_time, SPECTATORS[s->group][s->id].name, X);
            pthread_mutex_unlock(&spectator[s->group][s->id]);
        }
    }

    if (result == 0)
    {
        /*int value;
        sem_getvalue(&N, &value);
        printf("%s before N = %d ", s->name, value);*/
        sem_post(&N);
        //sem_getvalue(&N, &value);
        //printf("%s after N = %d ", s->name, value);
    }

    pthread_exit(NULL);
}

void wait_seat(spec *arg)
{
    if (arg->zone == 1)
    {
        pthread_t t1;
        pthread_create(&t1, NULL, enterzoneA, arg);
        pthread_join(t1, NULL);
    }
    else if (arg->zone == 2)
    {
        pthread_t t1, t2;
        pthread_create(&t1, NULL, enterzoneH, arg);
        pthread_create(&t2, NULL, enterzoneN, arg);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    }
    else if (arg->zone == 3)
    {
        pthread_t t1, t2, t3;
        pthread_create(&t1, NULL, enterzoneH, arg);
        pthread_create(&t2, NULL, enterzoneA, arg);
        pthread_create(&t3, NULL, enterzoneN, arg);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        pthread_join(t3, NULL);
    }
    if (SPECTATORS[arg->group][arg->id].state == waiting_seat)
    {

        pthread_mutex_lock(&spectator[arg->group][arg->id]);
        SPECTATORS[arg->group][arg->id].state = reached_gate;
        pthread_mutex_unlock(&spectator[arg->group][arg->id]);
        printf(BOLD YELLOW "t=%ld: %s couldn't get a seat\n" RESET, clock_time, arg->name);
    }
    if (SPECTATORS[arg->group][arg->id].state == reached_gate)
    {
        pthread_mutex_lock(&spectator[arg->group][arg->id]);
        SPECTATORS[arg->group][arg->id].state = waiting_friends;
        printf(BOLD LIGHT_PURPLE "t=%ld: %s is waiting for their friends at the exit\n" RESET, clock_time, arg->name);
        pthread_cond_wait(&group_wait[arg->group][arg->id], &spectator[arg->group][arg->id]);
        SPECTATORS[arg->group][arg->id].state = exited;
        pthread_mutex_unlock(&spectator[arg->group][arg->id]);
        pthread_exit(NULL);
    }
}

void *init_spectators(void *arg)
{
    spec *s = (spec *)arg;

    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        perror("error");
        exit(0);
    }
    ts.tv_sec += s->ctime;
    sem_timedwait(&spec_wait[s->group][s->id], &ts);
    printf(BOLD GREEN "t=%ld: %s reached the stadium\n" RESET, clock_time, s->name);

    pthread_mutex_lock(&spectator[s->group][s->id]);
    SPECTATORS[s->group][s->id].state = waiting_seat;
    pthread_mutex_unlock(&spectator[s->group][s->id]);

    while (s->state == waiting_seat)
    {
        wait_seat(s);
    }
    pthread_exit(NULL);
}