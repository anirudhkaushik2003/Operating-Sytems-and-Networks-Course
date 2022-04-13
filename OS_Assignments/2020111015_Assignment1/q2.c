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
    long long int parts = 0;
    long long int partnum = 0;
    for (int i = strlen(argv[argc - 3]) - 1; i >= 0; i--)
    {
        int temp = argv[argc - 3][i] - '0';
        for (int j = 0; j < strlen(argv[argc - 3]) - 1 - i; j++)
        {
            temp *= 10;
        }
        parts += temp;
    }
    for (int i = strlen(argv[argc - 2]) - 1; i >= 0; i--)
    {
        int temp = argv[argc - 2][i] - '0';
        for (int j = 0; j < strlen(argv[argc - 2]) - 1 - i; j++)
        {
            temp *= 10;
        }
        partnum += temp;
    }

    int f_s = open(file_name, O_RDONLY);
    int f_d = open("out.txt", O_CREAT | O_RDWR | O_TRUNC, 0600);

    long long file_size = lseek(f_s, 0, SEEK_END); //gives us the size of the file and sets f_s to end of the file

    long long int block_size = file_size / parts;
    printf("index = %lld\n", lseek(f_s, 0, SEEK_SET));
    long long int dest = lseek(f_s, block_size * (partnum - 1), SEEK_SET);
    printf("dest = %lld\n",dest);
    
    printf("lseek = %lld\n", lseek(f_s, block_size * (partnum), SEEK_SET));
    
    long long int move_size = 1024*16;
    
    char *BUFFER_READ = malloc(sizeof(char) * move_size);
    char *BUFFER_WRITE = malloc(sizeof(char) * move_size);
    lseek(f_d, 0, SEEK_SET); //sets f_d to the beginging of the destination file
    if (move_size > block_size)
    {
        move_size = block_size;
    }
    printf("move size  = %lld\n", move_size);
    printf("lseek = %lld\n", lseek(f_s, -move_size, SEEK_CUR));
    long long int total_bytes_read = 0;
    float start = clock();
    
    while (1)
    {
        /*break condition*/

        ssize_t bytes_read = read(f_s, BUFFER_READ, move_size);
        printf("bytes read = %lld\n", bytes_read);
        total_bytes_read += bytes_read;

        for (int i = 0; i < bytes_read; i++)
        {
            BUFFER_WRITE[i] = BUFFER_READ[bytes_read - i - 1];
        }
        write(f_d, BUFFER_WRITE, bytes_read);
        if (lseek(f_s, 0, SEEK_CUR) - bytes_read - move_size <= dest)
        {
            if (lseek(f_s, 0, SEEK_CUR) - bytes_read  == dest)
                break;
            else
                move_size = block_size - total_bytes_read ;
        }
        printf("move size  = %lld\n", move_size);
        printf("lseek = %lld\n", lseek(f_s, -move_size - bytes_read, SEEK_CUR));
    }
    float end1 = clock();
    printf("time taken = %f", (end1 - start) / 1000000.0);
}