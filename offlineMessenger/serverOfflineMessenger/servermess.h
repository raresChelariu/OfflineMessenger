#define PORT 7000
#define CAPACITY_THREADS 100
#define NO_LISTENS 30
#pragma once
#include "pch.h"
#include "threadContainer.h"

class ServerMess
{
public:
    ServerMess();
    bool loginDataValid(char* username, char* password);
    void AcceptingClients();

    void* ThreadStart(void*);
    int idTh = 0;
    pthread_t threads[CAPACITY_THREADS];
    vector<ThreadContainer*> allThreads;
    vector<string> linkedUsers;

    struct sockaddr_in srvSock;
    struct sockaddr_in cliSock;
    int srvDesc;
};

ServerMess::ServerMess()
{
    memset(&threads, 0, sizeof(pthread_t) * CAPACITY_THREADS);
    memset(&this->srvSock, 0, sizeof(sockaddr_in));
    memset(&this->srvSock, 0, sizeof(sockaddr_in));

    srvSock.sin_family = AF_INET;
    srvSock.sin_addr.s_addr = htonl(INADDR_ANY);
    srvSock.sin_port = htons(PORT);
    if (-1 == (this->srvDesc = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("Error on making socket!");
        exit(0);
    }
    int reuseServerAdress = 1;
    setsockopt(this->srvDesc, SOL_SOCKET, SO_REUSEADDR, &reuseServerAdress, sizeof(reuseServerAdress));
    if (-1 == bind(this->srvDesc, (sockaddr*)&this->srvSock, sizeof(sockaddr)))
    {
        printf("Error on trying to bind");
        exit(0);
    }
    if (-1 == listen(this->srvDesc, NO_LISTENS))
    {
        printf("Error on trying to listen!\n");
        exit(0);
    }
    printf("Listening to new clients!\n");
}
void ServerMess::AcceptingClients()
{
    printf("Accepting incoming connection requests");
    fflush(stdout);
    int cliDesc;
    memset(&cliSock, 0, sizeof(sockaddr_in));
    while (1)
    {
        int len = sizeof(cliSock);
        if (-1 == (cliDesc = accept(srvDesc, (sockaddr*)&cliSock, (socklen_t*)&len))) 
        {
            printf("Error on accepting new client!\n");
            fflush(stdout);
            continue;
        }

        ThreadContainer* t = new ThreadContainer(&threads[this->idTh++], cliDesc, &linkedUsers);
        perror("Hola");
        fflush(stdout);
        allThreads.push_back(t);
    }
}
