#include "history.h"

int hist_index = 0;
char **history_array;

void init_hist()
{
    char *root = malloc(sizeof(char) * 100);
    strcpy(root, getenv("HOME"));
    strcat(root, "/history.txt");
    FILE *fp = fopen(root, "r");
    if (!fp)
    {

        history_array = malloc(sizeof(char *) * HIST_LEN);

        for (int i = 0; i < HIST_LEN; i++)
        {
            history_array[i] = malloc(sizeof(char) * 100);
            strcpy(history_array[i], "");
        }
    }
    else
    {
        history_array = malloc(sizeof(char *) * HIST_LEN);
        char *temp = malloc(sizeof(char) * 100);
        int k = 0;
        int len;
        while (fgets(temp, 100, fp))
        {
            history_array[k] = malloc(sizeof(char) * 100);

            strcpy(history_array[k], temp);
            len = strlen(history_array[k]);
            history_array[k][len-1]='\0';
            hist_index++;
            hist_index = hist_index % HIST_LEN;
            k++;
        }
        for (int i = k; i < HIST_LEN; i++)
        {
            history_array[i] = malloc(sizeof(char) * 100);
            strcpy(history_array[i], "");
        }
        
        free(temp);
    }
}

void perma_store()
{
    char *root = malloc(sizeof(char) * 100);
    strcpy(root, getenv("HOME"));
    strcat(root, "/history.txt");
    FILE *fp = fopen(root, "w");
    if (!fp)
    {
        perror("File Error");
    }
    for (int i = (hist_index - 1) % HIST_LEN, j = 0; j < HIST_LEN; j++, i--)
    {
        if (i < 0)
            i += HIST_LEN;
        i = i % HIST_LEN;
        if (strcmp(history_array[i], "") != 0)
            fprintf(fp, "%s\n", history_array[i]);
    }
    fclose(fp);
    free(root);
}

void history(int num)
{

    for (int i = (hist_index - 1) % HIST_LEN, j = 0; j < num; j++, i--)
    {
        if (i < 0)
        {
            i += HIST_LEN;
        }
        i = i % HIST_LEN;
        if (strcmp(history_array[i], "") != 0)
            printf("%s\n", history_array[i]);
    }
}