#include <open62541/plugin/log_stdout.h>
#include <open62541/client_highlevel.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "ClientAgent.h"

using namespace std;
using namespace CLIENTAGENT;

UA_Boolean running = true;

static void stopHandler(int sign)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Received Ctrl-C");
    running = 0;
}

int main(int argc, char **argv)
{
    signal(SIGINT, stopHandler); /* catches ctrl-c */

    ClientAgent clientAgent = ClientAgent();
    clientAgent.clientConfig();

    const char *url = "opc.tcp://localhost:4840";

    // Put here the nodeId in Server to reading
    UA_NodeId nodeRead = UA_NODEID_NUMERIC(0, 54429);

    char *name = argv[1];
    char *pwd = argv[2];

    UA_StatusCode retVal = UA_STATUSCODE_BAD;

    while (retVal != UA_STATUSCODE_GOOD)
    {
        retVal = clientAgent.connectServer(url, name, pwd);
    }

    UA_UInt32 subId = clientAgent.createSubscription();
    clientAgent.monitorItens(subId, UA_NODEID_NUMERIC(0, 2253));



    /* Endless loop runAsync */
    while (running)
    {
        /* if already connected, this will return GOOD and do nothing */
        /* if the connection is closed/errored, the connection will be reset and then reconnected */
        /* Alternatively you can also use UA_Client_getState to get the current state */
        //clientAgent.connectServer(url, name, pwd);
        clientAgent.readValue(nodeRead);
        sleep(1);
    };

    /* Clean up */
    clientAgent.deleteClient();
    return EXIT_SUCCESS;
}