
#include <open62541/server.h>
#include <open62541/plugin/historydata/history_data_gathering_default.h>


#ifndef CREATORIM_H
#define CREATORIM_H

namespace CREATOR
{
    class CreatorIM
    {       // The class
    public: // Access specifier
        UA_Server *server;
        UA_StatusCode retVal;
        UA_HistoryDataGathering gathering;
        UA_HistorizingNodeIdSettings historicalSetting;


        CreatorIM(UA_Server *server, UA_StatusCode retVal);

        UA_NodeId createIntValue(UA_NodeId parentNodeId, UA_NodeId parentReferenceNodeId);
        void incrementalIntVal(UA_NodeId node, int initialValue, int finalValue);
        UA_NodeId createObject(UA_NodeId parentNodeId, UA_NodeId parentReferenceNodeId);
        UA_StatusCode createMethod(UA_NodeId dataType_input, UA_NodeId dataType_output,
                                   char *functionName, UA_MethodCallback method);
        void setCustomAccessControl(UA_ServerConfig *config, UA_UsernamePasswordLogin *userNamePW);

        UA_StatusCode addNewEventType();
        UA_StatusCode addGenerateEventMethod();
        UA_StatusCode createNodeSet(char * file);
        void setHistoricalData(UA_ServerConfig *config);
        UA_StatusCode setNodeToHistoricalData(UA_NodeId node);
        void setHistoricalConfig();


    };
}

#endif
