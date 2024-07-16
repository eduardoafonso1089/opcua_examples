
#include <open62541/server.h>

#ifndef CREATORIM_H
#define CREATORIM_H

namespace CREATOR
{
    class CreatorIM
    {       // The class
    public: // Access specifier
        UA_Server *server;
        UA_StatusCode retVal;

        CreatorIM(UA_Server *server, UA_StatusCode retVal);

        UA_NodeId createIntValue(UA_Int32 initialValue, UA_NodeId parentNodeId, UA_NodeId parentReferenceNodeId);
        void incrementalIntVal(UA_NodeId node, int initialValue, int finalValue);
    };
}

#endif
