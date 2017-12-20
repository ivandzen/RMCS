#ifndef ARRAYREF_H
#define ARRAYREF_H
#include <core/common/types.h>

template<typename T>
class ArrayRef
{
public:
    ArrayRef(T * data, Length_t size) :
        _data(data),
        _size(size)
    {}

    ArrayRef(const T * data, Length_t size) :
        _data(const_cast<T*>(data)),
        _size(size)
    {}

    ArrayRef(const ArrayRef<T> & other) :
        _data(other._data),
        _size(other._size)
    {}

    virtual ~ArrayRef() {}

    inline Length_t size() const { return _size; }

    inline void copyTo(T * target) const
    {
        memcpy(target, _data, _size);
    }

    inline void copyFrom(const T * source)
    {
        memcpy(_data, source, _size);
    }

    inline T * data() const { return _data; }

private:
    T * _data;
    Length_t _size;
};

#endif // ARRAYREF_H
