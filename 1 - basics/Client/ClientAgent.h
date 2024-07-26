
#include <open62541/server.h>

#ifndef CLIENTAGENT_H
#define CLIENTAGENT_H

namespace CLIENTAGENT
{
    class ClientAgent
    {       // The class
    public: // Access specifier
        UA_Client *client;
        UA_StatusCode retVal;

        ClientAgent();
        UA_StatusCode connectServer(const char * url);
        void deleteClient();
    };
}

#endif
