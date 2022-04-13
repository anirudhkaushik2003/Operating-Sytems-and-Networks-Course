#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *file_name = argv[argc - 1];

    int f_s = open(file_name, O_RDONLY);
    int f_d = open("out.txt", O_CREAT | O_RDWR | O_TRUNC, 0600);

    long long file_size = lseek(f_s, 0, SEEK_END); //gives us the size of the file and sets f_s to end of the file
    long long int move_size = 1024*16;
    printf("%lld\n", move_size);
    char *BUFFER_READ = malloc(sizeof(char) * move_size);
    char *BUFFER_WRITE = malloc(sizeof(char) * move_size);
    lseek(f_d, 0, SEEK_SET); //sets f_d to the beginging of the destination file
    if (move_size > file_size)
    {
        move_size = file_size;
    }
    lseek(f_s, -move_size, SEEK_CUR);
    float start = clock();

    while (1)
    {
        /*break condition*/

        ssize_t bytes_read = read(f_s, BUFFER_READ, move_size);

        for (int i = 0; i < bytes_read; i++)
        {
            BUFFER_WRITE[i] = BUFFER_READ[bytes_read - i - 1];
        }
        write(f_d, BUFFER_WRITE, bytes_read);
        if (lseek(f_s, 0, SEEK_CUR) - bytes_read - move_size <= 0)
        {
            if (lseek(f_s, 0, SEEK_CUR) - bytes_read  == 0)
                break;
            else
                move_size = lseek(f_s, 0, SEEK_CUR) - bytes_read;
        }
        lseek(f_s, -move_size - bytes_read, SEEK_CUR);
        
    }
    float end1 = clock();
    printf("time taken = %f", (end1 - start) / 1000000.0);
}