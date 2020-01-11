#include "servermess.h"
int main()
{
    ServerMess* server = new ServerMess();
    server->AcceptingClients();
    return 0;
}