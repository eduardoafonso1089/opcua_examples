#include <open62541/server.h>
#include <unistd.h>
#include "./CreatorIM.h"

/* Build Instructions (Linux)
 * - g++ server.cpp -lopen62541 -o server */
using namespace CREATOR;

CreatorIM::CreatorIM(UA_Server * server, UA_StatusCode retVal)
{
    this->server = server;
    this->retVal = retVal;
}

UA_NodeId CreatorIM::createIntValue(UA_Int32 initialValue, UA_NodeId parentNodeId, UA_NodeId parentReferenceNodeId)
{
    UA_NodeId outNodeId;
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Int32 myInteger = initialValue;

    UA_Variant_setScalarCopy(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
    attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Incremental Value");
    attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", "Incremental Value");
    UA_NodeId myIntegerNodeId = UA_NODEID_STRING_ALLOC(1, "incremental.value");
    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME_ALLOC(1, "Incremental Value");

    this->retVal = UA_Server_addVariableNode(server, myIntegerNodeId, parentNodeId,
                                             parentReferenceNodeId, myIntegerName,
                                             UA_NODEID_NULL, attr, NULL, &outNodeId);

    /* allocations on the heap need to be freed */
    UA_VariableAttributes_clear(&attr);
    UA_NodeId_clear(&myIntegerNodeId);
    UA_QualifiedName_clear(&myIntegerName);

    return outNodeId;
}


UA_StatusCode CreatorIM::incrementalIntVal(UA_NodeId node, int initialValue, int finalValue)
{
    int aux_initialValue = initialValue;
    UA_Variant value;
    UA_StatusCode checkStatus = UA_STATUSCODE_GOOD;

    while(1)
    {  
        UA_Variant_setScalar(&value, &aux_initialValue, &UA_TYPES[UA_NS0ID_INT64]);

        checkStatus |= UA_Server_writeValue(this->server, node, value);

        if( checkStatus != UA_STATUSCODE_GOOD)
        {
            this->retVal = UA_STATUSCODE_BAD;
        }

        aux_initialValue++;

        // Set value to 0 if initial value is bigger than final value
        aux_initialValue = aux_initialValue > finalValue? 0:aux_initialValue;

        sleep(1);
    }
    return this->retVal;
}
