#pragma once
#include "threadApp.h"

class ThreadContainer
{
public:
    ThreadContainer(pthread_t*, pthread_mutex_t* lock, int cliDesc, vector<string>*);
    static void* ThreadStartPoint(void*);
};

ThreadContainer::ThreadContainer(pthread_t* thread, pthread_mutex_t* lock,
                                int cliDesc, vector<string>* linkedUsers)
{
    ThreadData* data = (ThreadData*) malloc(sizeof(ThreadData));
    data->cliDesc = cliDesc;
    data->users = linkedUsers;
    data->lock = lock;
    perror("Hey ");
    fflush(stdout);
    pthread_create(thread, NULL, ThreadStartPoint, (void*)data);
}
void* ThreadContainer::ThreadStartPoint(void* arg)
{
    ThreadData* data = (ThreadData*) arg;
    ThreadApp myThread(data);
    perror("In thread start point:\n"); fflush(stdout);
    while (!myThread.Login())
        continue;

    while (1)
    {
        myThread.TreatCommand();
    }
    pthread_cancel(pthread_self());
    return NULL;
}