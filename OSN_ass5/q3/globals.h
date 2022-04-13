#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_SIZE 101


#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;
typedef struct dict dict;

struct dict
{
    int key;
    string value;
};

extern dict diks[MAX_SIZE];
extern pthread_mutex_t locks[MAX_SIZE];
extern pthread_mutex_t dict_lock;
extern int dict_locked;
extern int thread_pool_size;
extern pthread_t *thread_pool;

string concat(int key1, int key2);
string insert(int key, string value);
int check_key(int key);
string delet(int key);
string fetch(int key);
string update(int key, string value);

#endif