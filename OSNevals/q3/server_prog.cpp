#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>

#include "globals.h"
#include "myqueue.h"
/////////////////////////////
#include <iostream>
#include <assert.h>
#include <tuple>
using namespace std;
/////////////////////////////

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define ANSI_RESET "\x1b[0m"

int thread_pool_size;
pthread_t *thread_pool;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void *thread_function(void *arg);
void *handle_connection(void *arg);

queue<int *> thread_q;

typedef long long LL;

#define pb push_back
#define debug(x) cout << #x << " : " << x << endl
#define part cout << "-----------------------------------" << endl;

///////////////////////////////
#define MAX_CLIENTS 4
#define PORT_ARG 8001

const int initial_msg_len = 256;

////////////////////////////////////

const LL buff_sz = 1048576;

void *thread_function(void *arg)
{
    while (1)
    {
        int *pclient;
        pthread_mutex_lock(&m);
        while (thread_q.empty())
        {
            pthread_cond_wait(&c, &m);
        }
        pclient = thread_q.front();
        thread_q.pop();
        pthread_mutex_unlock(&m);
        handle_connection(pclient);

        /*if ((pclient = dequeue()) == NULL)
        {
            pthread_cond_wait(&c, &m);
            pclient = dequeue();
        }*/
    }
}

///////////////////////////////////////////////////
pair<string, int> read_string_from_socket(const int &fd, int bytes)
{
    std::string output;
    output.resize(bytes);

    int bytes_received = read(fd, &output[0], bytes - 1);
    //debug(bytes_received);
    if (bytes_received <= 0)
    {
        cerr << "Failed to read data from socket. \n";
    }

    output[bytes_received] = 0;
    output.resize(bytes_received);
    // debug(output);
    return {output, bytes_received};
}

int send_string_on_socket(int fd, const string &s)
{
    // debug(s.length());
    int bytes_sent = write(fd, s.c_str(), s.length());
    if (bytes_sent < 0)
    {
        cerr << "Failed to SEND DATA via socket.\n";
    }

    return bytes_sent;
}

///////////////////////////////

void *handle_connection(void *arg)
{
    int client_socket_fd = *(int *)arg;
    free(arg);
    // int client_socket_fd = *((int *)client_socket_fd_ptr);
    //####################################################

    int received_num, sent_num;

    /* read message from client */
    int ret_val = 1;

    while (true)
    {
        string cmd;
        tie(cmd, received_num) = read_string_from_socket(client_socket_fd, buff_sz);
        ret_val = received_num;
        // debug(ret_val);
        // printf("Read something\n");
        if (ret_val <= 0)
        {
            // perror("Error read()");
            printf("Server could not read msg sent from client\n");
            goto close_client_socket_ceremony;
        }
        //cout << "Client sent : " << cmd << endl;
        if (cmd == "exit")
        {
            cout << "Exit pressed by client" << endl;
            goto close_client_socket_ceremony;
        }
        string msg_to_send_back = cmd;
        vector<string> tokens;
        stringstream check1(cmd);
        string intermediate;
        while (getline(check1, intermediate, ' '))
        {
            tokens.push_back(intermediate);
        }
        if (tokens[0] == string("insert"))
        {
            int key;
            sscanf(tokens[1].c_str(), "%d", &key);
            msg_to_send_back = insert(key, tokens[2]);
        }
        else if (tokens[0] == string("delete"))
        {
            int key;
            sscanf(tokens[1].c_str(), "%d", &key);
            msg_to_send_back = delet(key);
        }
        else if (tokens[0] == string("update"))
        {
            int key;
            sscanf(tokens[1].c_str(), "%d", &key);
            msg_to_send_back = update(key, tokens[2]);
        }
        else if (tokens[0] == string("concat"))
        {
            int key, key2;
            sscanf(tokens[1].c_str(), "%d", &key);
            sscanf(tokens[2].c_str(), "%d", &key2);

            msg_to_send_back = concat(key, key2);
        }
        else if (tokens[0] == string("fetch"))
        {
            int key;
            sscanf(tokens[1].c_str(), "%d", &key);
            msg_to_send_back = fetch(key);
        }

        ////////////////////////////////////////
        // "If the server write a message on the socket and then close it before the client's read. Will the client be able to read the message?"
        // Yes. The client will get the data that was sent before the FIN packet that closes the socket.
        sleep(2);

        msg_to_send_back = string(":") + to_string(pthread_self()) + string(":") + msg_to_send_back;
        int sent_to_client = send_string_on_socket(client_socket_fd, msg_to_send_back);
        // debug(sent_to_client);
        if (sent_to_client == -1)
        {
            perror("Error while writing to client. Seems socket has been closed");
            goto close_client_socket_ceremony;
        }
        else
        {
            cout << msg_to_send_back << endl;
        }
    }

close_client_socket_ceremony:
    close(client_socket_fd);
    //printf(BRED "Disconnected from client" ANSI_RESET "\n");
    return NULL;
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        diks[i].key = -1;
        locks[i] = PTHREAD_MUTEX_INITIALIZER;
    }
    if (argc > 1)
    {
        sscanf(argv[1], "%d", &thread_pool_size);
    }
    else
        thread_pool_size = 1;

    int i, j, k, t, n;

    int wel_socket_fd, client_socket_fd, port_number;
    socklen_t clilen;

    struct sockaddr_in serv_addr_obj, client_addr_obj;
    /////////////////////////////////////////////////////////////////////////
    /* create socket */
    /*
    The server program must have a special door—more precisely,
    a special socket—that welcomes some initial contact 
    from a client process running on an arbitrary host
    */
    //get welcoming socket
    //get ip,port
    /////////////////////////
    thread_pool = (pthread_t *)malloc(sizeof(pthread_t) * thread_pool_size);
    for (int l = 0; l < thread_pool_size; l++)
    {
        pthread_create(&thread_pool[l], NULL, thread_function, NULL);
    }
    wel_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (wel_socket_fd < 0)
    {
        perror("ERROR creating welcoming socket");
        exit(-1);
    }

    //////////////////////////////////////////////////////////////////////
    /* IP address can be anything (INADDR_ANY) */
    bzero((char *)&serv_addr_obj, sizeof(serv_addr_obj));
    port_number = PORT_ARG;
    serv_addr_obj.sin_family = AF_INET;
    // On the server side I understand that INADDR_ANY will bind the port to all available interfaces,
    serv_addr_obj.sin_addr.s_addr = INADDR_ANY;
    serv_addr_obj.sin_port = htons(port_number); //process specifies port

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* bind socket to this port number on this machine */
    /*When a socket is created with socket(2), it exists in a name space
       (address family) but has no address assigned to it.  bind() assigns
       the address specified by addr to the socket referred to by the file
       descriptor wel_sock_fd.  addrlen specifies the size, in bytes, of the
       address structure pointed to by addr.  */

    //CHECK WHY THE CASTING IS REQUIRED
    if (bind(wel_socket_fd, (struct sockaddr *)&serv_addr_obj, sizeof(serv_addr_obj)) < 0)
    {
        perror("Error on bind on welcome socket: ");
        exit(-1);
    }
    //////////////////////////////////////////////////////////////////////////////////////

    /* listen for incoming connection requests */

    listen(wel_socket_fd, thread_pool_size);
    cout << "Server has started listening on the LISTEN PORT" << endl;
    clilen = sizeof(client_addr_obj);

    while (1)
    {
        /* accept a new request, create a client_socket_fd */
        /*
        During the three-way handshake, the client process knocks on the welcoming door
        of the server process. When the server “hears” the knocking, it creates a new door—
        more precisely, a new socket that is dedicated to that particular client. 
        */
        //accept is a blocking call
        //printf("Waiting for a new client to request for a connection\n");
        client_socket_fd = accept(wel_socket_fd, (struct sockaddr *)&client_addr_obj, &clilen);
        if (client_socket_fd < 0)
        {
            perror("ERROR while accept() system call occurred in SERVER");
            exit(-1);
        }

        //printf(BGRN "New client connected from port number %d and IP %s \n" ANSI_RESET, ntohs(client_addr_obj.sin_port), inet_ntoa(client_addr_obj.sin_addr));

        //pthread_t t;
        int *pclient = (int *)malloc(sizeof(int));
        *pclient = client_socket_fd;
        pthread_mutex_lock(&m);
        //enqueue(pclient);
        thread_q.push(pclient);
        pthread_cond_signal(&c);
        pthread_mutex_unlock(&m);

        //pthread_create(&t, NULL, handle_connection, pclient);
    }

    close(wel_socket_fd);
    return 0;
}