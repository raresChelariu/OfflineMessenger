#pragma once


using namespace std;
struct ThreadData
{
    int cliDesc;
    vector<string>* users;
    pthread_mutex_t* lock;
};
class ThreadApp
{
public:
    ThreadApp(ThreadData*);
    void TreatCommand();
    bool Login();
    FILE* GetFileDiscussion(const char* talker, const char* listener);
    void LoadChatLines(const char* talker, const char* listener);
    void AppendToFile(FILE* file, char* line);

    pthread_mutex_t* lock;
    int clientD;
    string LoggedUsername;
    vector<string>* users;
    vector<string> chatLines;
};

ThreadApp::ThreadApp(ThreadData* arg)
{
    this->clientD = arg->cliDesc;
    this->users = arg->users;
    this->lock = arg->lock;
}
bool ThreadApp::Login()
{
    char* msg = (char*) malloc(256);
    char* line = (char*) malloc(256);
    memset(msg, 0, 256);
    memset(line, 0, 256);

    read(this->clientD, msg, 256);
    printf("\n\n\n MESSAGE IN LOGIN %s!!", msg);
    fflush(stdout);
    FILE* fin = fopen("credentials.txt", "r");    

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
            memset(msg, 0, 256);
            strcpy(msg, "LOGIN_YES");

            write(this->clientD, msg, 256);
            LoggedUsername = string(lineUser);
            //printf("\n\n\n\n\n\n\n\n This is LoggedUsername %s\n\n\n\n\n\n", LoggedUsername.c_str()); fflush(stdout);
            
            pthread_mutex_lock(this->lock);
            fflush(stdout);
            users->push_back(string(lineUser));
            pthread_mutex_unlock(this->lock);
            printf("%s", msg);
            fflush(stdout);
            free(msg);
            free(line);
            return 1;
        }
    }
    fclose(fin);
    memset(msg, 0, 256);
    strcpy(msg, "LOGIN_NO");
    write(this->clientD, msg, 256);
    printf("%s", msg); fflush(stdout);
    free(msg);
    free(line);
    return 0;
}
void ThreadApp::TreatCommand()
{
    printf("Treat command func\n");
    fflush(stdout);
    char* msg = (char*) malloc(256);

    if (0 >= read(clientD, msg, 256))
    {
        printf("\n\n\n Client disconnected! \n\n\n"); fflush(stdout);
        pthread_mutex_lock(lock);
        int i;
        for (i = 0; i < users->size(); i++)
        {
            if ((*users)[i] == this->LoggedUsername)
                break;
        }
        int j, initSize = users->size();
        for (j = i+1; j < initSize; j++)
            (*users)[j-1] = (*users)[j]; 
        users->pop_back();
        fflush(stdout);
        pthread_mutex_unlock(lock);
        pthread_cancel(pthread_self());
    }
    if (!strcmp(msg, "getChatLines")) // TODO
    {
        read(clientD, msg, 256);
        LoadChatLines(this->LoggedUsername.c_str(), msg);

        int noLines = chatLines.size();
        printf("No lines of chat %d\n", (int)chatLines.size());
        for (int i = 0; i < chatLines.size(); i++)
            write(clientD, chatLines[i].c_str(), 256);   
        write(clientD, "getChatLinesEnd", 256);
    }
    if (!strcmp(msg, "sendMessage"))
    {
        char* sender = (char*) malloc(256);
        char* receiver = (char*) malloc(256);
        char* input = (char*) malloc(256);

        memset(sender, 0, 256);
        memset(receiver, 0, 256);
        memset(input, 0, 256);

        read(clientD, sender, 256);
        read(clientD, receiver, 256);
        read(clientD, input, 256);
        printf("\n\n\nSEND MESSAGE; SENDER : @%s@, RECEIVER: @%s@, INPUT: @%s@\n\n\n", sender, receiver, input);
        fflush(stdout);
        pthread_mutex_lock(lock);
        this->AppendToFile(this->GetFileDiscussion(sender, receiver), input);
        pthread_mutex_unlock(lock);
        free(sender);
        free(receiver);
        free(input);
    }
    if (!strcmp(msg, "getUsersOnline"))
    {
        pthread_mutex_lock(this->lock);
        printf("No users connected %d\n", (int)users->size()); fflush(stdout);
        for (auto it = users->begin(); users->end() != it; ++it)
        {
            write(clientD, it->c_str(), 256);
            printf("1111%s111\n", it->c_str());
        }      
        write(clientD,"getUsersOnlineEnd", 256);
        pthread_mutex_unlock(this->lock);
    }
    if (!strcmp(msg, "getUsersAll"))
    {
        pthread_mutex_lock(this->lock);
        FILE* fin = fopen("credentials.txt", "r");
        char* currUser = (char*) malloc(256);
        while (fgets(currUser, 256, fin) != NULL)
        {
            *strchr(currUser, ';') = 0;
            write(clientD, currUser, 256);       
        }
        write(clientD, "getUsersAllEnd", 256);
        free(currUser);
        fclose(fin);
        pthread_mutex_unlock(this->lock);
    }

    free(msg);
}
void ThreadApp::LoadChatLines(const char* talker, const char* listener)
{
    chatLines.clear();
    printf("Curr talker : %s ; Curr listener: %s\n", talker, listener);
    fflush(stdout);

    FILE* file = this->GetFileDiscussion(talker, listener);
    char line[256];
    char *p;    
    while (NULL != fgets(line, 256, file))
    {
        p  = strchr(line, '\n');
        if (NULL != p)
            *p = 0;
        chatLines.push_back(string(line));
    }
    fclose(file);
}
FILE* ThreadApp::GetFileDiscussion(const char* talker, const char* listener)
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
    sprintf(fileName, "_%s@%s.txt", personA, personB);
    FILE* myFile = fopen(fileName, "a+"); 
    return myFile;
}
void ThreadApp::AppendToFile(FILE* file, char* text)
{
    fprintf(file, "%s", text);
    fclose(file);
}