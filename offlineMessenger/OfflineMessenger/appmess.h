#pragma once
#include "client.h"

class AppMess
{
public:
    AppMess();
    static Client* client;
    static char username[256];
};

