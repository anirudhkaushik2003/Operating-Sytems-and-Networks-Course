#include "baywatch.h"
#include <termios.h>
#include <sys/ioctl.h>
#include <time.h>

// had to copy paste code for get key in order to get terminal into raw mode for processing this input, using it normally messed up my shell
//luckily it worked, first try zero deaths
char get_key()
{
    struct termios state;
    tcgetattr(0, &state);
    state.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &state);
    int buffer_count;
    ioctl(0, FIONREAD, &buffer_count);
    tcgetattr(0, &state);
    state.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &state);
    return (buffer_count == 0) ? '\0' : getchar();
}

void baywatch(token_mat args_mat)
{
    int opt;
    int interval = -1;
    // put ':' in the starting of the
    // string so that program can
    //distinguish between '?' and ':'
    int n = 0;

    while ((opt = getopt(args_mat.num_args, args_mat.args, ":n:")) != -1)
    {

        switch (opt)
        {
        case 'n':

            interval = atoi(optarg);
            break;
        }

        if (interval == -1)
        {
            perror("Wrong format");
            optind = 0;
            return;
        }
    }
    for (; optind < args_mat.num_args; optind++)
    {
        if (strcmp(args_mat.args[optind], "interrupt") == 0)
        {
            interrupt();
            int start = clock();
            int end;
            while (get_key() != 'q')
            {
                
                end = clock();
                if(((end - start)/CLOCKS_PER_SEC) >= interval)
                {
                  start = clock();
                  interrupt();
                }
            }
        }
        if (strcmp(args_mat.args[optind], "dirty") == 0)
        {
            dirty();
            int start = clock();
            int end;
            while (get_key() != 'q')
            {
                
                end = clock();
                if(((end - start)/CLOCKS_PER_SEC) >= interval)
                {
                  start = clock();
                  dirty();
                }
            }
        }
        optind++;
    }
    optind = 0;
}

void interrupt()
{
    FILE *fd = fopen("/proc/interrupts", "r");
    if (fd == NULL)
    {
        perror("(baywatch error)");

        return;
    }
    char *buffer = malloc(sizeof(char) * 100);
    for (int i = 0; i < 5; i++)
    {
        fgets(buffer, 100, fd);
        if (i == 0)
            printf("%s\n", buffer);
    }
    printf("\t%s\n", buffer + 5);
    fflush(stdout);
    free(buffer);
    fclose(fd);
}

void dirty()
{
    char *BUFF = malloc(sizeof(char) * 1000);
    FILE *fd = fopen("/proc/meminfo", "r");
    if (fd == NULL)
    {
        perror("(baywatch error)");

        return;
    }
    int i =0;
    while(fgets(BUFF, 100, fd))
    {
        if(i==16)
        {
            printf("%s",BUFF);
        }
        i++;
    }
    free(BUFF);
    fflush(stdout);
    fclose(fd);
}