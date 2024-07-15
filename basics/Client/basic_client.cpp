#include <open62541/plugin/log_stdout.h>
#include <signal.h>
#include <stdlib.h>
#include "ClientAgent.h"

using namespace std;
using namespace CLIENTAGENT;

UA_Boolean running = true;

static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Received Ctrl-C");
    running = 0;
}


int main(void) {
    signal(SIGINT, stopHandler); /* catches ctrl-c */

    ClientAgent clientAgent = ClientAgent();
    const char * url = "opc.tcp://localhost:4840";
    
    /* Endless loop runAsync */
    while(running) {
        /* if already connected, this will return GOOD and do nothing */
        /* if the connection is closed/errored, the connection will be reset and then reconnected */
        /* Alternatively you can also use UA_Client_getState to get the current state */
        clientAgent.connectServer(url);
        
    };

    /* Clean up */
    clientAgent.deleteClient();
    return EXIT_SUCCESS;
}