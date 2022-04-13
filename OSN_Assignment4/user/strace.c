#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fcntl.h"

int main(int argc, char **argv)
{

    strace(atoi(argv[1]));
    if (argc == 1)
    {
        sleep(10);
        exit(0);
    }
    else
    {
        exec(argv[2], argv + 2);
        printf("exec(): failed\n");
        exit(1);
    }
}