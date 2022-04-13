#include "globals.h"

job *jobs_list;
void status_sleepy()
{
    char pet[] = "            ,.  ,.\n            ||  ||\n           ,''--''.\n          : (-)(-) :\n         ,'   --   `.\n         :          :\n         :          :\n   -slp- `._m____m_,' \n";
    char pet2[] = "            ,.  ,.\n            ||  ||\n           ,''--''.\n          : (o)(o) :\n         ,'   --   `.\n         :          :\n         :          :\n   -slp- `._m____m_,' \n";
    char pet3[] = "            ,.  ,.\n            ||  ||\n           ,''--''.\n          : (o)(o) :\n         ,'    O   `.\n        :            :\n        :            :\n   -slp- `._m____m_,' \n";
    system("clear");
    int i;
    while (i < 2)
    {
        printf(GREEN);
        printf(BOLD);
        printf("%s\n", pet2);
        usleep(200000);
        system("clear");
        printf(LIGHT_PINK);
        printf(BOLD);
        printf("%s\n", pet3);
        usleep(200000);
        system("clear");
        printf(BLUE);
        printf(BOLD);
        printf("%s\n", pet);
        usleep(10000);
        system("clear");
        printf(RESET);
        i++;
    }
}
string root;
void handler(int sig)
{
    check_job();
}
int output_redirection;
int num_of_jobs;
int shell_pid;
int main()
{
    shell_pid = getpid();
    prctl(PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
    output_redirection = 0;
    num_of_jobs = 0;
    //Initialize
    status_sleepy();
    signal(SIGCHLD, handler);
    init_signals();
    root = Initialize_root();
    Init_joblist();
    setenv("OLDPWD", root.buf, 1);
    setenv("HOME", root.buf, 1);
    init_hist();

    string Input;
    Input.buf = (char *)malloc(sizeof(char) * MAX_TOKEN_LENGTH);
    while (1)
    {
        prompt(root);
        int i = 0;
        char temp;

        while (1)
        {
            
            if(feof(stdin))
            {
                exit(EXIT_SUCCESS);
            }
            temp = (char)getchar();
            if (temp != '\n')
            {
                Input.buf[i++] = temp;
                if (temp == ';')
                {
                    Input.buf[i - 1] = '\0';
                    Input.len = strlen(Input.buf);

                    printf(RESET);
                    fflush(stdout);

                    if (Input.len > 0)
                        tokenize_string(Input, root);
                    i = 0;
                }
            }

            else
            {

                Input.buf[i] = '\0';
                Input.len = strlen(Input.buf);

                printf(RESET);
                fflush(stdout);

                if (Input.len > 0)
                    tokenize_string(Input, root);
                break;
            }
        }
    }
}