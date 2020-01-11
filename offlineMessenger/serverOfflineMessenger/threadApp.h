#pragma once
#include "threadContainer.h"

using namespace std;
struct ThreadData
{
    int cliDesc;
    vector<string>* users;
};
class ThreadApp
{
public:
    ThreadApp(ThreadData*);
    void TreatCommand();
    bool Login();
    FILE* GetFileDiscussion(char* talker, char* listener);
    void LoadChatLines(char* talker, char* listener);
    void AppendToFile(FILE* file, char* line);
    
    int clientD;
    vector<string>* users;
    vector<string> chatLines;
};

ThreadApp::ThreadApp(ThreadData* arg)
{
    this->clientD = arg->cliDesc;
    this->users = arg->users;
}
bool ThreadApp::Login()
{
    char* msg = (char*) malloc(256);
    char* line = (char*) malloc(256);
    FILE* fin = fopen("credentials.txt", "r");

    read(this->clientD, msg, 256);

    char* user = strtok(msg, ";");
    char* pass = strtok(NULL, ";");

    char *lineUser, *linePass;
    while (fgets(line, 256, fin))
    {
        line[strlen(line)-1] = 0; // se pune si \n
        lineUser = strtok(line, ";");
        linePass = strtok(NULL, ";");
        
        if (!strcmp(user, lineUser) && !strcmp(pass, linePass))
        {
            fclose(fin);
            strcpy(msg, "LOGIN_YES");

            users->push_back(string(lineUser));
            write(this->clientD, msg, 256);
            printf("%s", msg);
            fflush(stdout);
            return 1;
        }
    }
    fclose(fin);
    strcpy(msg, "LOGIN_NO");
    printf("%s", msg);
    fflush(stdout);
    write(this->clientD, msg, 256);
    return 0;
}
void ThreadApp::TreatCommand()
{
    char* msg = (char*) malloc(256);
    read(clientD, msg, 256);
    if (!strcmp(msg, "receiverchanged")) // TODO
    {
        
    }
    if (!strcmp(msg, "refresh"))
    {
        
    }
}
void ThreadApp::LoadChatLines(char* talker, char* listener)
{
    FILE* file = this->GetFileDiscussion(talker, listener);
    char line[256];
    char *p;

    chatLines.clear();
    while (fgets(line, 256, file) != NULL)
    {
        p  = strchr(line, '\n');
        if (p != NULL)
            *p = 0;
        chatLines.push_back(string(line));
    }
    fclose(file);
}
FILE* ThreadApp::GetFileDiscussion(char* talker, char* listener)
{
    char fileName[256];

    char personA[128], personB[128];
    if (strcmp(talker, listener) < 0)
    {
        strcpy(personA, talker);
        strcpy(personB, listener);
    }
    else 
    {
        strcpy(personA, listener);
        strcpy(personB, talker);
    }
    sprintf(fileName, "%s@%s.txt", personA, personB);

    FILE* myFile = fopen(fileName, "a+"); 
    return myFile;
}
void ThreadApp::AppendToFile(FILE* file, char* text)
{
    fprintf(file, "%s\n", text);
    fclose(file);
}