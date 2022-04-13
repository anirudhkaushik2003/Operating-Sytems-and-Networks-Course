#include "myqueue.h"

node *head = NULL;
node *tail = NULL;

void enqueue(int *client_socket)
{
    node *newnode = (node *)malloc(sizeof(node));
    newnode->client_socket = client_socket;
    newnode->next = NULL;
    if (tail == NULL)
    {
        head = newnode;
    }
    else
    {
        tail->next = newnode;
    }
    tail = newnode;
}

int *dequeue()
{
    if (head == NULL)
        return NULL;
    else
    {
        int *result = head->client_socket;
        node *temp = head;
        head = head->next;
        if (head == NULL)
        {
            tail = NULL;
        }
        free(temp);
        return result;
    }
}