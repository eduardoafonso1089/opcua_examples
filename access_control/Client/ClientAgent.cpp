#include <open62541/client_config_default.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>
#include "./ClientAgent.h"
#include "common.h"

/* Build Instructions (Linux)
 * - g++ server.cpp -lopen62541 -o server */
using namespace CLIENTAGENT;

ClientAgent::ClientAgent()
{
    this->client = UA_Client_new();
    this->retVal = retVal;
    UA_ClientConfig *cc = UA_Client_getConfig(this->client);
    UA_ClientConfig_setDefault(cc);
}

UA_StatusCode ClientAgent::connectServer(const char *url, char *name, char *pwd)
{
    if (name == NULL && pwd == NULL)
    {
         this->retVal = UA_Client_connect(this->client, url);
        if (this->retVal != UA_STATUSCODE_GOOD)
        {
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                         "Not connected. Retrying to connect in 1 second");
            /* The connect may timeout after 1 second (see above) or it may fail immediately on network errors */
            /* E.g. name resolution errors or unreachable network. Thus there should be a small sleep here */
            sleep_ms(1000);
            // continue;
        }
    }
    else
    {
        this->retVal = UA_Client_connectUsername(this->client, url, name, pwd);
        if (this->retVal != UA_STATUSCODE_GOOD)
        {
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                         "Not connected. Retrying to connect in 1 second");
            /* The connect may timeout after 1 second (see above) or it may fail immediately on network errors */
            /* E.g. name resolution errors or unreachable network. Thus there should be a small sleep here */
            sleep_ms(1000);
            // continue;
        }
    }
    UA_Client_run_iterate(client, 1000);
    return this->retVal;
}

void ClientAgent::deleteClient()
{
    UA_Client_delete(this->client); /* Disconnects the client internally */
}

void ClientAgent::readValue(UA_NodeId node)
{
    UA_ReadRequest request;
    UA_ReadRequest_init(&request);
    UA_ReadResponse response;
    UA_ReadValueId nodesId;
    UA_ReadValueId_init(&nodesId);

    nodesId.attributeId = UA_ATTRIBUTEID_VALUE;
    nodesId.nodeId = node;
    request.nodesToReadSize = 1;
    request.nodesToRead = &nodesId;

    response = UA_Client_Service_read(this->client, request);

    if (response.responseHeader.serviceResult != UA_STATUSCODE_GOOD)
    {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Error %s \n", UA_StatusCode_name(response.responseHeader.serviceResult));
    }
    else
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Info %s -> Vale: %d  \n", UA_StatusCode_name(response.responseHeader.serviceResult), *(int *)response.results->value.data);
    }
}

void ClientAgent::clientConfig()
{
    UA_ClientConfig_setDefault(UA_Client_getConfig(this->client));
}

