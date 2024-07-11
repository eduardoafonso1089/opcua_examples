#include <open62541/server.h>
#include "./CreatorIM.h"

/* Build Instructions (Linux)
 * - g++ server.cpp -lopen62541 -o server */
using namespace CREATOR;

CreatorIM::CreatorIM(UA_Server * server, UA_StatusCode retVal)
{
    this->server = server;
    this->retVal = retVal;
}

UA_StatusCode CreatorIM::createIntValue(UA_Int32 initialValue, UA_NodeId parentNodeId, UA_NodeId parentReferenceNodeId)
{
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Int32 myInteger = initialValue;

    UA_Variant_setScalarCopy(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
    attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", "the answer");
    attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", "the answer");
    UA_NodeId myIntegerNodeId = UA_NODEID_STRING_ALLOC(1, "the.answer");
    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME_ALLOC(1, "the answer");

    this->retVal = UA_Server_addVariableNode(server, myIntegerNodeId, parentNodeId,
                                             parentReferenceNodeId, myIntegerName,
                                             UA_NODEID_NULL, attr, NULL, NULL);

    /* allocations on the heap need to be freed */
    UA_VariableAttributes_clear(&attr);
    UA_NodeId_clear(&myIntegerNodeId);
    UA_QualifiedName_clear(&myIntegerName);

    return this->retVal;
}
