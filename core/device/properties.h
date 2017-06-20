#ifndef PROPERTIES_H
#define PROPERTIES_H
#include <core/device/property.h>
#include <core/common/defaultnodes.h>

///////////////////////////////////////////////////////////////////////////////////

#define BOOL_PROP_RDONLY(name, getter) \
    ROProperty<bool> name = ROProperty<bool>(NODE_TYPE_BOOL_PROP_RDONLY, \
                                             #name, this, getter);

#define INT_PROP_RDONLY(name, getter) \
    ROProperty<int32_t> name = ROProperty<int32_t>(NODE_TYPE_INT_PROP_RDONLY, \
                                                   #name, this, getter);

#define FLOAT_PROP_RDONLY(name, getter) \
    ROProperty<float> name = ROProperty<float>(NODE_TYPE_FLOAT_PROP_RDONLY, \
                                               #name, this, getter);

///////////////////////////////////////////////////////////////////////////////////

#define BOOL_PROP(name, default_value, getter, setter) \
    RWProperty<bool> name = RWProperty<bool>(NODE_TYPE_BOOL_PROP, \
                                             #name, this, \
                                             getter, setter, \
                                             default_value);

#define INT_PROP(name, default_value, getter, setter) \
    RWProperty<int32_t> name = RWProperty<int32_t>(NODE_TYPE_INT_PROP, \
                                                   #name, this, \
                                                   getter, setter, \
                                                   default_value);

#define FLOAT_PROP(name, default_value, getter, setter) \
    RWProperty<float> name = RWProperty<float>(NODE_TYPE_FLOAT_PROP, \
                                               #name, this, \
                                               getter, setter, \
                                               default_value);

#endif // PROPERTIES_H
