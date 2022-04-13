#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char **argv)
{
    if (argc <= 2)
    {
        printf("Insufficient arguments\n");

        exit(0);
    }
    else
    {
        setpriority(atoi(argv[1]), atoi(argv[2]));

        exit(1);
    }
}