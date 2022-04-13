#include "ls.h"
int output_redirection;
char *red = RED;
char *green = GREEN;
char *bolden = BOLD;
char *purple = PURPLE;
char *blue = BLUE;
char *reset = RESET;

void makeLS(int num, char **args_mat)
{
    if (output_redirection == 1)
    {
        red = "";
        green = "";
        bolden = "";
        purple = "";
        blue = "";
        reset = "";
    }
    else
    {
        red = RED;
        green = GREEN;
        bolden = BOLD;
        purple = PURPLE;
        blue = BLUE;
        reset = RESET;
    }
    int opt;
    int cond = 0;
    // put ':' in the starting of the
    // string so that program can
    //distinguish between '?' and ':'
    int a = 0, l = 0;

    while ((opt = getopt(num, args_mat, "al")) != -1)
    {
        switch (opt)
        {
        case 'a':
            a = 1;
            break;
        case 'l':
            l = 1;
            break;
        }
    }
    if (num - optind > 1)
    {
        cond = 1;
    }
    else if (optind == num)
    {
        ls(".", a, l);
    }

    for (; optind < num; optind++)
    {
        if (cond == 1)
            printf("%s%s/%s:\n", blue, args_mat[optind], reset);
        if (args_mat[optind][0] == '~')
        {
            char *t = malloc(sizeof(char) * 100);
            for (int j = 0; j < strlen(args_mat[optind]); j++)
            {
                args_mat[optind][j] = args_mat[optind][j + 1];
            }
            strcpy(t, getenv("HOME"));
            strcat(t, args_mat[optind]);
            strcpy(args_mat[optind], t);
        }
        ls(args_mat[optind], a, l);
        if (cond == 1)
            printf("\n");
    }
    optind = 0;
}

void ls(const char *input, int a, int l)
{
    char *bold = bolden;
    struct dirent **files;

    int num;
    char *color;
    char buf[256];
    char *ptr;
    num = scandir(input, &files, NULL, alphasort);
    if (num == -1)
    {
        struct stat check;

        if (stat(input, &check) != -1)
        {
            files = malloc(sizeof(struct dirent *));
            files[0] = malloc(sizeof(struct dirent));

            num = 1;
            strcpy(files[0]->d_name, input);
        }
        else
        {
            printf("%s", red);
            printf("Name = %s\n", input);
            perror("Error");
            return;
        }
    }
    char *temp = malloc(sizeof(char) * 2560);
    int total = 0;
    if (l == 1)
    {
        for (int i = 0; i < num; i++)
        {
            if (a != 1 && files[i]->d_name[0] == '.')
                continue;
            strcpy(temp, input);
            strcat(temp, "/");
            strcat(temp, files[i]->d_name);

            ptr = realpath(temp, buf);

            struct stat totalStat;
            stat(buf, &totalStat);

            total += totalStat.st_blocks; // these are 512 byte blocks which are different from 1024 byte blocks that ls counts, so the result is double
        }
        printf("total %d\n", total);
    }

    for (int i = 0; i < num; i++)
    {
        if (a != 1 && files[i]->d_name[0] == '.')
            continue;

        strcpy(temp, input);
        strcat(temp, "/");
        strcat(temp, files[i]->d_name);

        ptr = realpath(temp, buf);

        struct stat fileStat;
        stat(buf, &fileStat);

        time_t t = fileStat.st_mtim.tv_sec;
        struct tm lt;
        localtime_r(&t, &lt);
        char timebuf[80];
        char timebuf2[40];
        char timebuf3[40];
        strftime(timebuf, sizeof(timebuf), "%b %d %R", &lt);
        strftime(timebuf2, sizeof(timebuf), "%Y", &lt);
        t = time(NULL);
        localtime_r(&t, &lt);
        strftime(timebuf3, sizeof(timebuf), "%Y", &lt);
        if (strcmp(timebuf2, timebuf3) != 0)
        {
            ;
        }
        else
        {
            strcpy(timebuf2, "");
        }

        struct passwd *pw = getpwuid(fileStat.st_uid);
        struct group *gr = getgrgid(fileStat.st_gid);
        if (l == 1)
        {
            if (files[i]->d_name[0] == '.')
                color = purple;
            else if (files[i]->d_type == DT_DIR)
                color = blue;
            else if (fileStat.st_mode & S_IXUSR)
                color = green;
            else
                color = "";
            if (S_ISLNK(fileStat.st_mode))
                printf("l");
            else if (S_ISREG(fileStat.st_mode))
                printf("-");
            else if (S_ISDIR(fileStat.st_mode))
                printf("d");
            else if (S_ISCHR(fileStat.st_mode))
                printf("c");
            else if (S_ISBLK(fileStat.st_mode))
                printf("b");
            else if (S_ISFIFO(fileStat.st_mode))
                printf("f");
            else if (S_ISSOCK(fileStat.st_mode))
                printf("s");
            printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
            printf(" %2ld %s %s %5ld %s %s %s%s%s\n", fileStat.st_nlink, pw->pw_name, gr->gr_name, (long)fileStat.st_size, timebuf, timebuf2, bold, color, files[i]->d_name);
            printf("%s", reset);
        }
        else
        {
            if (files[i]->d_name[0] == '.')
                color = purple;
            else if (files[i]->d_type == DT_DIR)
                color = blue;
            else if (fileStat.st_mode & S_IXUSR)
                color = green;
            else
                color = "";
            printf("%s%s%s\n", bold, color, files[i]->d_name);
            printf("%s", reset);
        }
    }
    free(temp);
}
