#include "cd.h"

void changedir(char *path_name, char *root)
{
    int cond = 0;
    if (path_name == NULL)
    {
        setenv("OLDPWD", getpwd().buf, 1);
        if (chdir(root) != 0)
        {
            printf(RED);
            perror("Error");
            printf(RESET);
        }
        cond = 1;
    }
    else if (strcmp(path_name, "~") == 0)
    {
        setenv("OLDPWD", getpwd().buf, 1);
        if (chdir(root) != 0)
        {
            printf(RED);
            perror("Error");
            printf(RESET);
        }
        cond = 1;
    }
    else if (strcmp(path_name, "-") == 0)
    {
        char *prv = getpwd().buf;
        if (chdir(getenv("OLDPWD")) != 0)
        {
            printf(RED);
            perror("Error");
            printf(RESET);
        }
        cond = 1;
        setenv("OLDPWD", prv, 1);
    }
    else
        setenv("OLDPWD", getpwd().buf, 1);
    if (cond != 1 && chdir(path_name) != 0)
    {
        printf(RED);
        perror("Error");
        printf(RESET);
    }
}