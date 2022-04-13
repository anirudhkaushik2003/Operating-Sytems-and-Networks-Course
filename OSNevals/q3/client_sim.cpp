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
#include <bits/stdc++.h>

/////////////////////////////
#include <pthread.h>
#include <iostream>
#include <semaphore.h>
#include <assert.h>
#include <queue>
#include <vector>
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

typedef long long LL;
const LL MOD = 1000000007;
#define part cout << "-----------------------------------" << endl;
#define pb push_back
#define debug(x) cout << #x << " : " << x << endl

///////////////////////////////
#define SERVER_PORT 8001
////////////////////////////////////

const LL buff_sz = 1048576;
///////////////////////////////////////////////////
typedef struct argu argu;
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;
struct argu
{
    int time;
    int id;
    char cmd[100];
};

pair<string, int> read_string_from_socket(int fd, int bytes)
{
    std::string output;
    output.resize(bytes);

    int bytes_received = read(fd, &output[0], bytes - 1);
    // debug(bytes_received);
    if (bytes_received <= 0)
    {
        cerr << "Failed to read data from socket. Seems server has closed socket\n";
        // return "
        exit(-1);
    }

    // debug(output);
    output[bytes_received] = 0;
    output.resize(bytes_received);

    return {output, bytes_received};
}

int send_string_on_socket(int fd, const string &s)
{
    // cout << "We are sending " << s << endl;
    int bytes_sent = write(fd, s.c_str(), s.length());
    // debug(bytes_sent);
    // debug(s);
    if (bytes_sent < 0)
    {
        cerr << "Failed to SEND DATA on socket.\n";
        // return "
        exit(-1);
    }

    return bytes_sent;
}

int get_socket_fd(struct sockaddr_in *ptr)
{
    struct sockaddr_in server_obj = *ptr;

    // socket() creates an endpoint for communication and returns a file
    //        descriptor that refers to that endpoint.  The file descriptor
    //        returned by a successful call will be the lowest-numbered file
    //        descriptor not currently open for the process.
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Error in socket creation for CLIENT");
        exit(-1);
    }
    /////////////////////////////////////////////////////////////////////////////////////
    int port_num = SERVER_PORT;

    memset(&server_obj, 0, sizeof(server_obj)); // Zero out structure
    server_obj.sin_family = AF_INET;
    server_obj.sin_port = htons(port_num); //convert to big-endian order

    // Converts an IP address in numbers-and-dots notation into either a
    // struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
    //https://stackoverflow.com/a/20778887/6427607

    /////////////////////////////////////////////////////////////////////////////////////////
    /* connect to server */

    if (connect(socket_fd, (struct sockaddr *)&server_obj, sizeof(server_obj)) < 0)
    {
        perror("Problem in connecting to the server");
        exit(-1);
    }

    //part;
    // printf(BGRN "Connected to server\n" ANSI_RESET);
    // part;
    return socket_fd;
}
////////////////////////////////////////////////////////

void begin_process(int request_n, string to_send)
{
    struct sockaddr_in server_obj;
    int socket_fd = get_socket_fd(&server_obj);

    //cout << "Connection to server successful" << endl;

    send_string_on_socket(socket_fd, to_send);
    int num_bytes_read;
    string output_msg;
    tie(output_msg, num_bytes_read) = read_string_from_socket(socket_fd, buff_sz);
    pthread_mutex_lock(&print_lock);
    cout << request_n << output_msg << endl;
    pthread_mutex_unlock(&print_lock);
    close(socket_fd);

    // part;
}

void *init_thread(void *arg)
{
    argu argument = *(argu *)arg;
    sleep(argument.time);
    begin_process(argument.id, string(argument.cmd));
    //pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[])
{
    int m;
    scanf("%d", &m);
    int time[m];
    string command[m];
    pthread_t t[m];
    string temp;

    getline(cin, temp);
    argu *arg = (argu *)malloc(sizeof(argu));

    for (int i = 0; i < m; i++)
    {
        getline(cin, temp);

        vector<string> tokens;
        stringstream check1(temp);
        string intermediate;
        while (getline(check1, intermediate, ' '))
        {
            tokens.push_back(intermediate);
        }
        sscanf(tokens[0].c_str(), "%d", &time[i]);
        for (int j = 1; j < tokens.size(); j++)
        {
            command[i] += tokens[j];
            if (j != tokens.size() - 1)
            {
                command[i] += " ";
            }
        }

        arg->time = time[i];
        arg->id = i;
        strcpy(arg->cmd, command[i].c_str());
        pthread_create(&t[i], NULL, init_thread, arg);
        //init_thread(arg);
    }
    
    for (int i = 0; i < m; i++)
    {
        pthread_join(t[i], NULL);
    }

    return 0;
}