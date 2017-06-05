#ifndef DEFAULTNODES_H
#define DEFAULTNODES_H

enum NodeType
{
    NODE_TYPE_DEFAULT,

    NODE_TYPE_UINT8_IPARAM,
    NODE_TYPE_UINT16_IPARAM,
    NODE_TYPE_UINT32_IPARAM,
    NODE_TYPE_FLOAT32_IPARAM,
    NODE_TYPE_FLOAT64_IPARAM,

    NODE_TYPE_UINT8_OPARAM,
    NODE_TYPE_UINT16_OPARAM,
    NODE_TYPE_UINT32_OPARAM,
    NODE_TYPE_FLOAT32_OPARAM,
    NODE_TYPE_FLOAT64_OPARAM,

    NODE_TYPE_ENUM_PROP_RDONLY,
    NODE_TYPE_BOOL_PROP_RDONLY,
    NODE_TYPE_INT_PROP_RDONLY,
    NODE_TYPE_FLOAT_PROP_RDONLY,

    NODE_TYPE_ENUM_PROP,
    NODE_TYPE_BOOL_PROP,
    NODE_TYPE_INT_PROP,
    NODE_TYPE_FLOAT_PROP
};

#endif //DEFAULTNODES_H