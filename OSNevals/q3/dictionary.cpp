#include "globals.h"

dict diks[MAX_SIZE];
pthread_mutex_t locks[MAX_SIZE];
pthread_mutex_t dict_lock;
int dict_locked;

string insert(int key, string value)
{
    int cond = 0;
    pthread_mutex_lock(&locks[key]);
    if (diks[key].key == -1)
    {
        diks[key].key = key;
        diks[key].value = value;
        cond = 1;
    }
    pthread_mutex_unlock(&locks[key]);
    if (cond == 1)
    {
        return string("Insertion successful");
    }
    else
        return string("Key already exists");
}

string concat(int key1, int key2)
{
    int cond = 0;
    string val1, val2;
    pthread_mutex_lock(&locks[key1]);
    if (diks[key1].key != -1)
    {

        val1 = diks[key1].value;
        cond = 1;
    }
    pthread_mutex_unlock(&locks[key1]);
    if (cond == 0)
    {
        return string("Concat failed as at least one of the keys does not exist");
    }
    cond = 0;
    pthread_mutex_lock(&locks[key2]);
    if (diks[key2].key != -1)
    {

        val2 = diks[key2].value;
        cond = 1;
    }
    pthread_mutex_unlock(&locks[key2]);
    if (cond == 0)
    {
        return string("Concat failed as at least one of the keys does not exist");
    }
    else if (cond == 1)
    {
        diks[key1].value = val1 + val2;
        diks[key2].value = val2 + val1;
    }
    return diks[key2].value;
}

string update(int key, string value)
{
    int cond = 0;
    pthread_mutex_lock(&locks[key]);
    if (diks[key].key != -1)
    {
        diks[key].value = value;
        cond = 1;
    }
    pthread_mutex_unlock(&locks[key]);
    if (cond == 1)
    {
        return diks[key].value;
    }
    else
        return string("Key does not exist");
}

string delet(int key)
{
    int cond = 0;
    pthread_mutex_lock(&locks[key]);
    if (diks[key].key != -1)
    {
        diks[key].key = -1;
        diks[key].value.clear();
        cond = 1;
    }
    pthread_mutex_unlock(&locks[key]);
    if (cond == 1)
    {
        return string("Deletion successful");
    }
    else
        return string("No such key exists");
}

string fetch(int key)
{
    int cond = 0;
    pthread_mutex_lock(&locks[key]);
    if (diks[key].key != -1)
    {
        cond = 1;
    }
    pthread_mutex_unlock(&locks[key]);
    if (cond == 1)
    {
        return diks[key].value;
    }
    else
        return string("Key does not exist");
}
