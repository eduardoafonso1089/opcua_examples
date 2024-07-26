/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <open62541/server.h>
#include <open62541/plugin/log_stdout.h>
#include <thread>
#include "./CreatorIM.h"

using namespace std;
using namespace CREATOR;


int main()
{

    UA_StatusCode retval;
    UA_Server *server = UA_Server_new();
    UA_StatusCode retVal = UA_STATUSCODE_GOOD;
    UA_NodeId nodeIncremented;

    CreatorIM opcuaServer = CreatorIM(server, retVal);

    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);


    nodeIncremented = opcuaServer.createIntValue(100, parentNodeId, parentReferenceNodeId);
    //Launch thread of incremental value
    thread th1(&CreatorIM::incrementalIntVal, opcuaServer, nodeIncremented, 0, 100); 

    if (opcuaServer.retVal != UA_STATUSCODE_GOOD)
    {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Error %s \n", UA_StatusCode_name(opcuaServer.retVal));
    }
    else
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Info %s \n", UA_StatusCode_name(opcuaServer.retVal));
    }

    retval = UA_Server_runUntilInterrupt(opcuaServer.server);

    UA_Server_delete(opcuaServer.server);

    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}