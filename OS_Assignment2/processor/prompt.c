#include "prompt.h"
#include "../globals.h"
#include <pwd.h>

struct utsname unameData;

void prompt(string root)
{

    register struct passwd *pw;
    register uid_t uid;
    uid = getuid();
    pw = getpwuid(uid);

    string cwd = current_dir(root);
    if (uname(&unameData) != 0)
    {
        printf(RESET);
        printf(RED);
        perror("Error");
        printf(RESET);
    }

    char *sysname = malloc(sizeof(char) * 100);
    strcpy(sysname, unameData.nodename);

    printf(BLUE);
    printf("<%s@%s", pw->pw_name, sysname);
    printf(RESET);
    printf(":");
    printf(GREEN);
    printf("%s", cwd.buf);
    printf(BLUE);
    printf("> ");
    printf(YELLOW);

    free(cwd.buf);
    free(sysname);
}