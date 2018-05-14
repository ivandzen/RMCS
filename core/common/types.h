#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <vector>
#include <string>
#include <limits>
#include <functional>

typedef uint8_t ParamDataID;

template<typename T>
struct ParamData
{
    ParamDataID id;
    T value;

    inline bool isActual(ParamDataID last_id) const
    {
        int delta_id = (id - last_id + std::numeric_limits<ParamDataID>::max()) %
                        std::numeric_limits<ParamDataID>::max();
        return (delta_id > 0) && (delta_id < 7);
    }
};

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

#pragma pack(push, 1)

#pragma pack(pop)


#endif //TYPES_H
