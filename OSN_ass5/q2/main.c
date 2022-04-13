#include "globals.h"

sem_t H, A, N;
spec **SPECTATORS;
int *groups;
goals *GOALS;

time_t clock_time;
sem_t **spec_locks;
sem_t **spec_wait;
int goal_index = 0;
int G_H = 0, G_A = 0;

pthread_cond_t **group_wait;

int X, G; // spectating timer, number of groups
int g;    //number of goal events

pthread_cond_t **match_wait;
pthread_mutex_t **spectator;

void *init_timer(void *arg)
{
    struct timespec ts;
    time_t timer;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        exit(0);
    }
    timer = ts.tv_sec;
    time_t origin = timer;
    goal_index = 0;
    while (1)
    {
        if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        {
            perror("error");
            exit(0);
        }
        if (ts.tv_sec - timer >= 1)
        {
            clock_time = ts.tv_sec - origin;
            //printf("%ld\n", clock_time);
            timer = ts.tv_sec;
            if (goal_index < g)
            {
                if (clock_time == GOALS[goal_index].time)
                {

                    if (GOALS[goal_index].team == 'A')
                    {
                        srand(time(NULL));
                        if (((float)rand() / (float)RAND_MAX) <= GOALS[goal_index].prob)
                        {
                            G_A += 1;
                            printf("t=%ld: Team A has scored their %d goal\n", clock_time, G_A);
                        }
                        else
                        {
                            printf("t=%ld: Team A has missed the chance to score their %d goal\n", clock_time, G_A + 1);
                        }
                    }
                    else if (GOALS[goal_index].team == 'H')
                    {
                        srand(time(NULL));
                        if (((float)rand() / (float)RAND_MAX) <= GOALS[goal_index].prob)
                        {
                            G_H += 1;
                            printf("t=%ld: Team H has scored their %d goal\n", clock_time, G_H);
                        }
                        else
                        {
                            printf("t=%ld: Team H has missed the chance to score their %d goal\n", clock_time, G_H + 1);
                        }
                    }

                    goal_index++;
                }
            }
            for (int i = 0; i < G; i++)
            {
                for (int j = 0; j < groups[i]; j++)
                {
                    if (clock_time >= (SPECTATORS[i][j].etime))
                    {
                        while (SPECTATORS[i][j].state == watching_match)
                        {
                            pthread_cond_signal(&match_wait[i][j]);
                        }
                    }
                    else
                    {
                        if (SPECTATORS[i][j].zone == 2)
                        {
                            if (SPECTATORS[i][j].R == G_A)
                            {
                                while (SPECTATORS[i][j].state == watching_match)
                                {
                                    pthread_mutex_lock(&spectator[i][j]);
                                    SPECTATORS[i][j].enraged = 1;
                                    pthread_mutex_unlock(&spectator[i][j]);
                                    pthread_cond_signal(&match_wait[i][j]);
                                }
                            }
                        }
                        else if (SPECTATORS[i][j].zone == 1)
                        {
                            if (SPECTATORS[i][j].R == G_H)
                            {
                                while (SPECTATORS[i][j].state == watching_match)
                                {
                                    pthread_mutex_lock(&spectator[i][j]);
                                    SPECTATORS[i][j].enraged = 1;
                                    pthread_mutex_unlock(&spectator[i][j]);
                                    pthread_cond_signal(&match_wait[i][j]);
                                }
                            }
                        }
                    }
                }
            }
            for (int i = 0; i < G; i++)
            {
                int cond = 0;
                for (int j = 0; j < groups[i]; j++)
                {
                    if (SPECTATORS[i][j].state == waiting_friends)
                    {
                        cond = 1;
                    }
                    else
                    {
                        cond = 0;
                        break;
                    }
                }
                if (cond == 1)
                {
                    for (int j = 0; j < groups[i]; j++)
                    {
                        while (SPECTATORS[i][j].state == waiting_friends)
                            pthread_cond_signal(&group_wait[i][j]);
                    }
                    printf(BOLD LIGHT_PINK "t=%ld: Group %d is leaving for dinner\n" RESET, clock_time, i + 1);
                }
            }
        }
    }
}

int main()
{

    int n1, n2, n3;
    int total = 0;
    scanf("%d %d %d", &n1, &n2, &n3);
    sem_init(&H, 0, n1);
    sem_init(&A, 0, n2);
    sem_init(&N, 0, n3);
    scanf("%d", &X);
    scanf("%d", &G);
    SPECTATORS = malloc(sizeof(spec *) * G);
    groups = malloc(sizeof(int) * G);
    spec_locks = malloc(sizeof(sem_t *) * G);
    spec_wait = malloc(sizeof(sem_t *) * G);
    match_wait = malloc(sizeof(pthread_cond_t *) * G);
    spectator = malloc(sizeof(pthread_mutex_t *) * G);
    group_wait = malloc(sizeof(pthread_cond_t) * G);
    for (int i = 0; i < G; i++)
    {
        int k; //number of ppl in the grp
        scanf("%d", &k);
        groups[i] = k;
        total += k;
        SPECTATORS[i] = malloc(sizeof(spec) * k);
        spec_locks[i] = malloc(sizeof(sem_t) * k);
        spec_wait[i] = malloc(sizeof(sem_t) * k);
        match_wait[i] = malloc(sizeof(pthread_cond_t) * k);
        spectator[i] = malloc(sizeof(pthread_mutex_t) * k);
        group_wait[i] = malloc(sizeof(pthread_cond_t) * k);

        for (int j = 0; j < k; j++)
        {
            char zone;
            SPECTATORS[i][j].name = malloc(sizeof(char) * 100);
            sem_init(&spec_locks[i][j], 0, 1);
            sem_init(&spec_wait[i][j], 0, 0);
            pthread_cond_init(&match_wait[i][j], NULL);
            pthread_mutex_init(&spectator[i][j], NULL);
            pthread_cond_init(&group_wait[i][j], NULL);

            scanf("%s %c %d %d %d", SPECTATORS[i][j].name, &zone, &SPECTATORS[i][j].ctime, &SPECTATORS[i][j].P, &SPECTATORS[i][j].R);
            if (zone == 'H')
                SPECTATORS[i][j].zone = 2;
            if (zone == 'A')
                SPECTATORS[i][j].zone = 1;
            if (zone == 'N')
                SPECTATORS[i][j].zone = 3;
            SPECTATORS[i][j].group = i;
            SPECTATORS[i][j].state = not_reached;
            SPECTATORS[i][j].id = j;
            SPECTATORS[i][j].enraged = 0;
        }
    }
    scanf("%d", &g);
    GOALS = malloc(sizeof(goals) * g);
    for (int i = 0; i < g; i++)
    {
        getchar();
        scanf("%c %d %f", &GOALS[i].team, &GOALS[i].time, &GOALS[i].prob);
    }

    pthread_t t;
    pthread_create(&t, NULL, init_timer, NULL);

    pthread_t **threads;
    threads = malloc(sizeof(pthread_t *) * G);

    for (int i = 0; i < G; i++)
    {
        threads[i] = malloc(sizeof(pthread_t) * groups[i]);
        for (int j = 0; j < groups[i]; j++)
        {
            int rc = pthread_create(&threads[i][j], NULL, init_spectators, &SPECTATORS[i][j]);
            assert(rc == 0);
        }
    }
    for (int i = 0; i < G; i++)
    {
        for (int j = 0; j < groups[i]; j++)
        {
            pthread_join(threads[i][j], NULL);
        }
    }
}