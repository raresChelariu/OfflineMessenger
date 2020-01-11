#include "client.h"
#define LOCALHOST "127.0.0.1"
#define PORT 7000
Client::Client() {
    memset(&serverSock, 0, sizeof(sockaddr_in));
    serverSock.sin_family = AF_INET;
    serverSock.sin_port = htons(PORT);
    serverSock.sin_addr.s_addr = inet_addr(LOCALHOST);
}
void Client::connect() {
    if (-1 == (sockDescr = socket(AF_INET, SOCK_STREAM, 0))) {
        printf("Error on creating socket!\n");
        return;
    }
    if (-1 == ::connect(sockDescr, (sockaddr*)&serverSock, sizeof(sockaddr))) {
        printf("Error on attempting to connect!\n");
        return;
    }
}
