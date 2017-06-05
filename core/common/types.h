#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <limits>
#include <assert.h>
#include <functional>

typedef uint16_t    NodeType_t;
typedef uint16_t    NodeID_t;
typedef uint8_t     CtlReq_t;
typedef uint8_t     Data;
typedef Data *      DataPtr;
typedef uint16_t    Length_t;
typedef uint16_t    NodeStatus_t;
typedef uint32_t    Time_t;

#define NodeIDNull (std::numeric_limits<NodeID_t>::max())
#define StreamIDNull (std::numeric_limits<StreamID_t>::max())

enum NodeStatus
{
    NODE_STAT_UNDEFINED,
    NODE_STAT_OK,
    NODE_STAT_ERR
};

template<typename T>
struct ParamData
{
    uint8_t id;
    T value;
};


#endif //TYPES_H
