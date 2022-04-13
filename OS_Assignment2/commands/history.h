#ifndef HISTORY_H
#define HISTORY_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

extern char **history_array;
#define HIST_LEN 20
extern int hist_index;

void init_hist();
void history(int num);
void perma_store();

#endif