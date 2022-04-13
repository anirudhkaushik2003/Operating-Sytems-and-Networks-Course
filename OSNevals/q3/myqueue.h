#include <stdio.h>
#include <stdlib.h>

#ifndef MYQUEUE_H
#define MYQUEUE_H
typedef struct node node;

struct node
{
    node *next;
    int *client_socket;
};

int *dequeue();
void enqueue(int *client_socket);
#endif