#include <open62541/server.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server_config_default.h>
#include <open62541/plugin/accesscontrol_default.h>
#include <open62541/plugin/historydata/history_data_backend_memory.h>
#include <open62541/plugin/historydata/history_data_gathering_default.h>
#include <open62541/plugin/historydata/history_database_default.h>
#include <open62541/plugin/historydatabase.h>
#include <unistd.h>
#include "./CreatorIM.h"
#include <functional>
#include <open62541/plugin/nodesetloader.h>

/* Build Instructions (Linux)
 * - g++ server.cpp -lopen62541 -o server */
using namespace CREATOR;

static UA_NodeId eventType;

CreatorIM::CreatorIM(UA_Server *server, UA_StatusCode retVal)
{
    this->server = server;
    this->retVal = retVal;
}

UA_NodeId CreatorIM::createIntValue(UA_NodeId parentNodeId, UA_NodeId parentReferenceNodeId)
{
    UA_NodeId outNodeId;
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Int32 myInteger = 0;

    UA_Variant_setScalarCopy(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
    attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Incremental Value");
    attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", "Incremental Value");
    // Support history read to be reported to clients
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
    attr.historizing = true;
    // UA_NodeId myIntegerNodeId = UA_NODEID_STRING_ALLOC(1, "incremental.value");
    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME_ALLOC(1, "Incremental Value");

    this->retVal = UA_Server_addVariableNode(this->server, UA_NODEID_NULL, parentNodeId,
                                             parentReferenceNodeId, myIntegerName,
                                             UA_NODEID_NULL, attr, NULL, &outNodeId);

    /* allocations on the heap need to be freed */
    UA_VariableAttributes_clear(&attr);
    // UA_NodeId_clear(&myIntegerNodeId);
    UA_QualifiedName_clear(&myIntegerName);

    return outNodeId;
}

void CreatorIM::incrementalIntVal(UA_NodeId node, int initialValue, int finalValue)
{
    int aux_initialValue = initialValue;
    UA_DateTime dt;

    UA_WriteValue wv;
    UA_WriteValue_init(&wv);
    wv.value.hasStatus = true;
    wv.value.hasValue = true;
    wv.value.value.type = &UA_TYPES[UA_TYPES_INT64];
    wv.value.hasServerTimestamp = true;
    wv.value.hasSourceTimestamp = true;
    wv.nodeId = node;
    wv.attributeId = UA_ATTRIBUTEID_VALUE;

    UA_Variant value;

    UA_StatusCode checkStatus = UA_STATUSCODE_GOOD;

    while (1)
    {
        UA_Variant_setScalar(&wv.value.value, &aux_initialValue, &UA_TYPES[UA_NS0ID_INT64]);

        wv.value.sourceTimestamp = UA_DateTime_now();


        //checkStatus |= UA_Server_writeValue(this->server, node, value);

        checkStatus |= UA_Server_write(this->server, &wv);

        if (checkStatus != UA_STATUSCODE_GOOD)
        {
            this->retVal = UA_STATUSCODE_BAD;
        }

        aux_initialValue++;

        // Set value to 0 if initial value is bigger than final value
        aux_initialValue = aux_initialValue > finalValue ? 0 : aux_initialValue;

        sleep(1);
    }
}

UA_NodeId CreatorIM::createObject(UA_NodeId parentNodeId, UA_NodeId parentReferenceNodeId)
{
    UA_NodeId outNodeId;
    UA_ObjectAttributes attr = UA_ObjectAttributes_default;

    attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Folder Value");
    attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", "Folder Value");
    UA_NodeId myIntegerNodeId = UA_NODEID_STRING_ALLOC(1, "folder.value");
    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME_ALLOC(1, "Folder Value");

    this->retVal = UA_Server_addObjectNode(this->server, myIntegerNodeId, parentNodeId,
                                           parentReferenceNodeId, myIntegerName,
                                           UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), attr, NULL, &outNodeId);

    /* allocations on the heap need to be freed */
    UA_ObjectAttributes_clear(&attr);
    UA_NodeId_clear(&myIntegerNodeId);
    UA_QualifiedName_clear(&myIntegerName);

    return outNodeId;
}

UA_StatusCode CreatorIM::createMethod(UA_NodeId dataType_input, UA_NodeId dataType_output,
                                      char *functionName, UA_MethodCallback method)
{
    UA_StatusCode retVal;
    UA_Argument inputArgument[2];
    UA_Argument_init(&inputArgument[0]);
    inputArgument[0].description = UA_LOCALIZEDTEXT((char *)"en-US", (char *)"First Arg");
    inputArgument[0].name = UA_STRING((char *)"x");
    inputArgument[0].dataType = dataType_input;
    inputArgument[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArgument[1]);
    inputArgument[1].description = UA_LOCALIZEDTEXT((char *)"en-US", (char *)"Second Arg");
    inputArgument[1].name = UA_STRING((char *)"y");
    inputArgument[1].dataType = dataType_input;
    inputArgument[1].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char *)"en-US", (char *)"Result");
    outputArgument.name = UA_STRING((char *)"Output");
    outputArgument.dataType = dataType_output;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes helloAttr = UA_MethodAttributes_default;
    helloAttr.description = UA_LOCALIZEDTEXT((char *)"en-US", functionName);
    helloAttr.displayName = UA_LOCALIZEDTEXT((char *)"en-US", functionName);
    helloAttr.executable = true;
    helloAttr.userExecutable = true;
    retVal = UA_Server_addMethodNode(server, UA_NODEID_STRING(1, functionName),
                                     UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                     UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                     UA_QUALIFIEDNAME(1, functionName),
                                     helloAttr, method,
                                     2, inputArgument, 1, &outputArgument, NULL, NULL);

    if (retVal != UA_STATUSCODE_GOOD)
    {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Error %s \n", UA_StatusCode_name(retVal));
    }
    else
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Info %s \n", UA_StatusCode_name(retVal));
    }

    return retVal;
}

UA_Boolean
allowAddNode(UA_Server *server, UA_AccessControl *ac,
             const UA_NodeId *sessionId, void *sessionContext,
             const UA_AddNodesItem *item)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Called allowAddNode");
    return UA_TRUE;
}

UA_Boolean
allowAddReference(UA_Server *server, UA_AccessControl *ac,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_AddReferencesItem *item)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Called allowAddReference");

    return UA_TRUE;
}

UA_Boolean
allowDeleteNode(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_DeleteNodesItem *item)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Called allowDeleteNode");

    return UA_FALSE; // Do not allow deletion from client
}

UA_Boolean
allowDeleteReference(UA_Server *server, UA_AccessControl *ac,
                     const UA_NodeId *sessionId, void *sessionContext,
                     const UA_DeleteReferencesItem *item)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Called allowDeleteReference");
    return UA_TRUE;
}

void CreatorIM::setCustomAccessControl(UA_ServerConfig *config, UA_UsernamePasswordLogin *userNamePW)
{
    /* Use the default AccessControl plugin as the starting point */
    UA_Boolean allowAnonymous = false;
    UA_String encryptionPolicy =
        config->securityPolicies[config->securityPoliciesSize - 1].policyUri;
    config->accessControl.clear(&config->accessControl);
    UA_AccessControl_default(config, allowAnonymous, &encryptionPolicy, 2, userNamePW);

    /* Override accessControl functions for nodeManagement */
    config->accessControl.allowAddNode = allowAddNode;
    config->accessControl.allowAddReference = allowAddReference;
    config->accessControl.allowDeleteNode = allowDeleteNode;
    config->accessControl.allowDeleteReference = allowDeleteReference;
}

UA_StatusCode
CreatorIM::addNewEventType()
{
    UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "SimpleEventType");
    attr.description = UA_LOCALIZEDTEXT("en-US", "The simple event type we created");
    return UA_Server_addObjectTypeNode(this->server, UA_NODEID_NULL,
                                       UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE),
                                       UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                       UA_QUALIFIEDNAME(0, "SimpleEventType"),
                                       attr, NULL, &eventType);
}

UA_StatusCode
setUpEvent(UA_Server *server, UA_NodeId *outId)
{

    UA_StatusCode retval = UA_Server_createEvent(server, eventType, outId);
    if (retval != UA_STATUSCODE_GOOD)
    {
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                       "createEvent failed. StatusCode %s", UA_StatusCode_name(retval));
        return retval;
    }

    /* Set the Event Attributes */
    /* Setting the Time is required or else the event will not show up in UAExpert! */
    UA_DateTime eventTime = UA_DateTime_now();
    UA_Server_writeObjectProperty_scalar(server, *outId, UA_QUALIFIEDNAME(0, "Time"),
                                         &eventTime, &UA_TYPES[UA_TYPES_DATETIME]);

    UA_UInt16 eventSeverity = 100;
    UA_Server_writeObjectProperty_scalar(server, *outId, UA_QUALIFIEDNAME(0, "Severity"),
                                         &eventSeverity, &UA_TYPES[UA_TYPES_UINT16]);

    UA_LocalizedText eventMessage = UA_LOCALIZEDTEXT("en-US", "An event has been generated.");
    UA_Server_writeObjectProperty_scalar(server, *outId, UA_QUALIFIEDNAME(0, "Message"),
                                         &eventMessage, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);

    UA_String eventSourceName = UA_STRING("Server");
    UA_Server_writeObjectProperty_scalar(server, *outId, UA_QUALIFIEDNAME(0, "SourceName"),
                                         &eventSourceName, &UA_TYPES[UA_TYPES_STRING]);

    return UA_STATUSCODE_GOOD;
}

UA_StatusCode generateEventMethodCallback(UA_Server *server,
                                          const UA_NodeId *sessionId, void *sessionHandle,
                                          const UA_NodeId *methodId, void *methodContext,
                                          const UA_NodeId *objectId, void *objectContext,
                                          size_t inputSize, const UA_Variant *input,
                                          size_t outputSize, UA_Variant *output)
{

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Creating event");

    /* set up event */
    UA_NodeId auxEventType;
    UA_NodeId_copy(&eventType, &auxEventType);

    UA_StatusCode retval = setUpEvent(server, &auxEventType);
    if (retval != UA_STATUSCODE_GOOD)
    {
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                       "Creating event failed. StatusCode %s", UA_StatusCode_name(retval));
        return retval;
    }

    retval = UA_Server_triggerEvent(server, auxEventType,
                                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER),
                                    NULL, UA_TRUE);
    if (retval != UA_STATUSCODE_GOOD)
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                       "Triggering event failed. StatusCode %s", UA_StatusCode_name(retval));

    UA_NodeId_clear(&auxEventType);

    return retval;
}

UA_StatusCode CreatorIM::addGenerateEventMethod()
{
    UA_MethodAttributes generateAttr = UA_MethodAttributes_default;
    generateAttr.description = UA_LOCALIZEDTEXT("en-US", "Generate an event.");
    generateAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Generate Event");
    generateAttr.executable = true;
    generateAttr.userExecutable = true;

    UA_Server_addMethodNode(this->server, UA_NODEID_NUMERIC(1, 62541),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "Generate Event"),
                            generateAttr, &generateEventMethodCallback,
                            0, NULL, 0, NULL, NULL, NULL);
}

UA_StatusCode CreatorIM::createNodeSet(char *file)
{
    if (UA_StatusCode_isBad(UA_Server_loadNodeset(this->server, file, NULL)))
    {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Nodeset %s could not be loaded, exit\n", file);
        return EXIT_FAILURE;
    }
    else
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Nodeset %s loaded\n", file);
        return EXIT_SUCCESS;
    }
}

void CreatorIM::setHistoricalData(UA_ServerConfig *config)
{
    /* We need a gathering for the plugin to constuct.
     * The UA_HistoryDataGathering is responsible to collect data and store it to the database.
     * We will use this gathering for one node, only. initialNodeIdStoreSize = 1
     * The store will grow if you register more than one node, but this is expensive. */
    UA_HistoryDataGathering gathering = UA_HistoryDataGathering_Default(1);

    /* We set the responsible plugin in the configuration. UA_HistoryDatabase is
     * the main plugin which handles the historical data service. */
    config->historyDatabase = UA_HistoryDatabase_default(gathering);

    this->gathering = gathering;
}

void CreatorIM::setHistoricalConfig()
{
    UA_HistorizingNodeIdSettings setting;
    setting.historizingBackend = UA_HistoryDataBackend_Memory(3, 100);

    setting.maxHistoryDataResponseSize = 100;

    setting.historizingUpdateStrategy = UA_HISTORIZINGUPDATESTRATEGY_VALUESET;

    this->historicalSetting = setting; 
}


UA_StatusCode CreatorIM::setNodeToHistoricalData(UA_NodeId node)
{
    UA_StatusCode retval;

    retval = this->gathering.registerNodeId(this->server, this->gathering.context, &node, this->historicalSetting);

    if (retval != UA_STATUSCODE_GOOD)
    {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Error in historical registration %s", UA_StatusCode_name(retval));
    }
    else
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                    "Success in historical registration node %s", UA_StatusCode_name(retval));
    }
    return retval;
}
