#pragma once
#include "client.h"
#include <string>
class AppMess
{
public:
    AppMess();
    static Client* client;
    static std::string username;
};

