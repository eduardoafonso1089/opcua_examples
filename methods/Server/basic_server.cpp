/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <open62541/server.h>
#include <open62541/plugin/log_stdout.h>
#include <thread>
#include "./CreatorIM.h"

using namespace std;
using namespace CREATOR;

static UA_StatusCode
intSumMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    UA_Int32 *inputStr1 = (UA_Int32*)input[0].data;
    UA_Int32 *inputStr2 = (UA_Int32*)input[1].data;

    UA_Int32 tmp = *inputStr1 + *inputStr2;

    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_INT32]);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "The sum was done!");

    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
multFloatMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    UA_Float *inputStr1 = (UA_Float*)input[0].data;
    UA_Float *inputStr2 = (UA_Float*)input[1].data;

    UA_Float tmp = *inputStr1 * *inputStr2;

    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_FLOAT]);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "The multiplication was done!");

    return UA_STATUSCODE_GOOD;
}

int main()
{

    UA_StatusCode retval;
    UA_Server *server = UA_Server_new();
    UA_StatusCode retVal = UA_STATUSCODE_GOOD;
    UA_NodeId nodeIncremented1, nodeIncremented2, nodeIncremented3, nodeIncremented4;

    CreatorIM opcuaServer = CreatorIM(server, retVal);

    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);

    // Create folder parent 
    UA_NodeId parentFolder = opcuaServer.createObject(parentNodeId, parentReferenceNodeId);

    // Create variables nestled to folder parent
    nodeIncremented1 = opcuaServer.createIntValue(parentFolder, parentReferenceNodeId);
    nodeIncremented2 = opcuaServer.createIntValue(parentFolder, parentReferenceNodeId);
    nodeIncremented3 = opcuaServer.createIntValue(parentFolder, parentReferenceNodeId);
    nodeIncremented4 = opcuaServer.createIntValue(parentFolder, parentReferenceNodeId);

    //Launch threads of incremental value
    thread th1(&CreatorIM::incrementalIntVal, opcuaServer, nodeIncremented1, 0, 100); 
    thread th2(&CreatorIM::incrementalIntVal, opcuaServer, nodeIncremented2, 0, 10); 
    thread th3(&CreatorIM::incrementalIntVal, opcuaServer, nodeIncremented3, 0, 15); 
    thread th4(&CreatorIM::incrementalIntVal, opcuaServer, nodeIncremented4, 0, 200); 


    if (opcuaServer.retVal != UA_STATUSCODE_GOOD)
    {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Error %s \n", UA_StatusCode_name(opcuaServer.retVal));
    }
    else
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Info %s \n", UA_StatusCode_name(opcuaServer.retVal));
    }

    UA_NodeId dataType_input = UA_TYPES[UA_TYPES_INT32].typeId;
    UA_NodeId dataType_output = UA_TYPES[UA_TYPES_INT32].typeId;
    char * functionName = "Int sum";

    opcuaServer.createMethod(dataType_input, dataType_output, functionName, intSumMethodCallback);

    dataType_input = UA_TYPES[UA_TYPES_FLOAT].typeId;
    dataType_output = UA_TYPES[UA_TYPES_FLOAT].typeId;
    functionName = "Float mult";

    opcuaServer.createMethod(dataType_input, dataType_output, functionName, multFloatMethodCallback);

    retval = UA_Server_runUntilInterrupt(opcuaServer.server);

    UA_Server_delete(opcuaServer.server);

    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}