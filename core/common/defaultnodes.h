#ifndef DEFAULTNODES_H
#define DEFAULTNODES_H
#include <core/common/types.h>

enum NodeType
{
    NODE_TYPE_DEFAULT               = 0x00,

    NODE_TYPE_INT8_IPARAM           = 0x10,
    NODE_TYPE_INT16_IPARAM          = 0x11,
    NODE_TYPE_INT32_IPARAM          = 0x12,
    NODE_TYPE_FLOAT32_IPARAM        = 0x13,
    NODE_TYPE_FLOAT64_IPARAM        = 0x14,
    NODE_TYPE_RAW_INT8_ICHANNEL     = 0x15,
    NODE_TYPE_RAW_INT16_ICHANNEL    = 0x16,
    NODE_TYPE_RAW_INT32_ICHANNEL    = 0x17,

    NODE_TYPE_INT8_OPARAM           = 0x20,
    NODE_TYPE_INT16_OPARAM          = 0x21,
    NODE_TYPE_INT32_OPARAM          = 0x22,
    NODE_TYPE_FLOAT32_OPARAM        = 0x23,
    NODE_TYPE_FLOAT64_OPARAM        = 0x24,
    NODE_TYPE_RAW_INT8_OCHANNEL     = 0x25,
    NODE_TYPE_RAW_INT16_OCHANNEL    = 0x26,
    NODE_TYPE_RAW_INT32_OCHANNEL    = 0x27,

    NODE_TYPE_ENUM_PROP_RDONLY      = 0x30,
    NODE_TYPE_BOOL_PROP_RDONLY      = 0x31,
    NODE_TYPE_INT_PROP_RDONLY       = 0x32,
    NODE_TYPE_FLOAT_PROP_RDONLY     = 0x33,

    NODE_TYPE_ENUM_PROP             = 0x40,
    NODE_TYPE_BOOL_PROP             = 0x41,
    NODE_TYPE_INT_PROP              = 0x42,
    NODE_TYPE_FLOAT_PROP            = 0x43,

};

inline bool isIParamType(NodeType_t type) {
    return (type == NODE_TYPE_INT8_IPARAM) ||
           (type == NODE_TYPE_INT16_IPARAM) ||
           (type == NODE_TYPE_INT32_IPARAM) ||
           (type == NODE_TYPE_FLOAT32_IPARAM) ||
           (type == NODE_TYPE_FLOAT64_IPARAM);
}

inline bool isOParamType(NodeType_t type) {
    return (type == NODE_TYPE_INT8_OPARAM) ||
           (type == NODE_TYPE_INT16_OPARAM) ||
           (type == NODE_TYPE_INT32_OPARAM) ||
           (type == NODE_TYPE_FLOAT32_OPARAM) ||
           (type == NODE_TYPE_FLOAT64_OPARAM);
}

inline bool isParamType(NodeType_t type) {
    return isIParamType(type) || isOParamType(type);
}

inline bool isROPropertyType(NodeType_t type) {
    return (type == NODE_TYPE_BOOL_PROP_RDONLY) ||
           (type == NODE_TYPE_INT_PROP_RDONLY) ||
           (type == NODE_TYPE_FLOAT_PROP_RDONLY);
}

inline bool isRWPropertyType(NodeType_t type) {
    return (type == NODE_TYPE_BOOL_PROP) ||
           (type == NODE_TYPE_INT_PROP) ||
           (type == NODE_TYPE_FLOAT_PROP);
}

inline bool isPropertyType(NodeType_t type) {
    return isROPropertyType(type) || isRWPropertyType(type);
}

#endif //DEFAULTNODES_H
