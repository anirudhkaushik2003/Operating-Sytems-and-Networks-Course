#include "globals.h"
#include <signal.h>

lab *LABS;
student *STUDENTS;
course *COURSES;

int num_courses;
int num_students;
int num_labs;

pthread_mutex_t pref_lock[MAX_COURSES];
int times;

void *time_update(void *arg)
{
    times = 0;
    while (check_students() == 1)
    {

        sleep(1);
        if (times % 4 == 0)
        {
            for (int i = 0; i < num_students; i++)
            {
                if (STUDENTS[i].state != exited_simulation && STUDENTS[i].state != selected_course)
                    printf("student %d %s %d\n", STUDENTS[i].id, COURSES[STUDENTS[i].pref[STUDENTS[i].current_pref]].name, STUDENTS[i].state);
            }
            for (int i = 0; i < num_courses; i++)
            {
                printf("course:%s state:%d\n", COURSES[i].name, COURSES[i].state);
                printf("check labs = %d\n",check_labs(&COURSES[i]));
            }
            for (int i = 0; i < num_labs; i++)
            {
                printf("Lab %s state:%d\n", LABS[i].name, LABS[i].state);
                for (int j = 0; j < LABS[i].num_TA; j++)
                {
                    printf("    TA %d state %d\n", LABS[i].Tas[j].id, LABS[i].Tas[j].state);
                }
            }
        }

        times++;
    }
}

int main()
{

    pthread_t timer;
    scanf("%d %d %d", &num_students, &num_labs, &num_courses);
    LABS = malloc(sizeof(lab) * num_labs);
    STUDENTS = malloc(sizeof(student) * num_students);
    COURSES = malloc(sizeof(course) * num_courses);
    pthread_t s[num_students], l[num_labs], c[num_courses];
    init_TA_locks();
    init_student_locks();
    init_tutorials();

    init_lab_locks();

    for (int i = 0; i < num_courses; i++)
    {
        COURSES[i].id = i;
        COURSES[i].state = waiting_TA;
        COURSES[i].name = malloc(sizeof(char) * 100);
        scanf("%s %f %d %d", COURSES[i].name, &COURSES[i].interest, &COURSES[i].course_max_slot, &COURSES[i].num_labs);
        COURSES[i].Labs = malloc(sizeof(int) * COURSES[i].num_labs);
        for (int j = 0; j < COURSES[i].num_labs; j++)
        {
            scanf("%d", &COURSES[i].Labs[j]);
        }
    }
    for (int i = 0; i < num_students; i++)
    {
        int pref1, pref2, pref3;
        scanf("%f %d %d %d %d", &STUDENTS[i].calibre, &pref1, &pref2, &pref3, &STUDENTS[i].time);
        STUDENTS[i].id = i;
        STUDENTS[i].pref[0] = pref1;
        STUDENTS[i].pref[1] = pref2;
        STUDENTS[i].pref[2] = pref3;

        STUDENTS[i].current_pref = 0;
        STUDENTS[i].state = not_registered;
    }
    for (int i = 0; i < num_labs; i++)
    {
        LABS[i].name = malloc(sizeof(char) * 100);
        LABS[i].id = i;
        scanf("%s %d %d", LABS[i].name, &LABS[i].num_TA, &LABS[i].quota);
        create_lab(&LABS[i]);
    }
    //pthread_create(&timer, NULL, time_update, NULL);
    for (int i = 0; i < num_labs; i++)
    {
        int rc = pthread_create(&l[i], NULL, init_lab, &LABS[i]);
        assert(rc == 0);
    }
    for (int i = 0; i < num_courses; i++)
    {
        int rc = pthread_create(&c[i], NULL, init_course, &COURSES[i]);
        assert(rc == 0);
    }
    for (int i = 0; i < num_students; i++)
    {
        pthread_create(&s[i], NULL, init_student, &STUDENTS[i]);
    }

    for (int i = 0; i < num_labs; i++)
    {
        pthread_join(l[i], NULL);
    }
    for (int i = 0; i < num_students; i++)
    {
        pthread_join(s[i], NULL);
    }
    for (int i = 0; i < num_courses; i++)
    {
        pthread_join(c[i], NULL);
    }
}