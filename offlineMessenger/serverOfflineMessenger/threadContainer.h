#pragma once
#include "threadApp.h"


using namespace std;

class ThreadContainer
{
public:
    ThreadContainer(pthread_t*, int cliDesc, vector<string>*);
    static void* ThreadStartPoint(void*);
};

ThreadContainer::ThreadContainer(pthread_t* thread, int cliDesc, vector<string>* linkedUsers)
{
    ThreadData* data = (ThreadData*) malloc(sizeof(ThreadData));
    data->cliDesc = cliDesc;
    data->users = linkedUsers;
    perror("Hey ");
    fflush(stdout);
    pthread_create(thread, NULL, ThreadStartPoint, (void*)data);
}
void* ThreadContainer::ThreadStartPoint(void* arg)
{
    ThreadData* data = (ThreadData*) arg;
    ThreadApp myThread(data);
    if (!myThread.Login())
        return NULL;
    while (1)
    {
        myThread.TreatCommand();
    }
    pthread_cancel(pthread_self());
    return NULL;
}