#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#define PORT 7000
#define LOCALHOST "127.0.0.1"

class Client
{
public:
    Client();
    sockaddr_in serverSock;
    int sockDescr;

    void connect();
};



