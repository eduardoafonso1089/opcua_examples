
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
        UA_StatusCode connectServer(const char *url, char *name, char *pwd);
        void readValue(UA_NodeId node);
        void deleteClient();
        void clientConfig();
        UA_UInt32 createSubscription();
        UA_StatusCode monitorItens(UA_UInt32 subId,UA_NodeId monitoredNode);

    };
}

#endif
