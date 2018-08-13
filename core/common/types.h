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

typedef uint16_t    NodeType_t;
typedef uint16_t    NodeID_t;
typedef uint8_t     CtlReq_t;
typedef uint8_t     Data;
typedef Data *      DataPtr;
typedef uint16_t    Length_t;
typedef uint16_t    NodeStatus_t;
typedef uint32_t    Time_t;

typedef uint32_t ParamDataID;
typedef uint8_t  ParamDataIdx;

template<typename T>
struct _ParamData
{
public:
    typedef ParamDataID IDType;
    typedef ParamDataIdx IdxType;

    _ParamData(Data * ptr = nullptr, IdxType size = 0) :
        _ptr(reinterpret_cast<Content*>(ptr)),
        _size(size)
    {}

    static uint8_t byteSize(IdxType count) { return sizeof(Content) + sizeof(T) * count; }

    inline bool isValid() const { return (_ptr != nullptr) && (_size != 0); }

    inline IdxType size() const { return _size; }

    inline void setID(IDType id) { _ptr->id = id; }

    inline void incID() { ++_ptr->id; }

    inline IDType getID() const { return _ptr->id; }

    inline T get(IdxType idx) const
    {
        assert(idx < _size);
        return _ptr->data[idx];
    }

    inline void set(IdxType idx, T value)
    {
        assert(idx < _size);
        _ptr->data[idx] = value;
    }

    inline T * ptr() const { return _ptr->data; }

private:
    typedef struct
    {
        IDType 	id;
        T       data[];
    }
    Content;

    Content *   _ptr;
    IdxType     _size;
};


#define NodeIDNull (std::numeric_limits<NodeID_t>::max())
#define StreamIDNull (std::numeric_limits<StreamID_t>::max())

enum NodeStatus
{
    NODE_STAT_UNDEFINED,
    NODE_STAT_OK,
    NODE_STAT_ERR
};

template<typename T>
class FIFO
{
public:
    static const uint32_t DEFAULT_SIZE = 64;

    FIFO(size_t size = DEFAULT_SIZE) :
        _pushIdx(0),
        _popIdx(0)
    {
        _data.resize(size);
    }

    inline int size() const { return _data.size(); }

    inline void resize(uint32_t size)
    {
        _pushIdx = 0;
        _popIdx = 0;
        _data.resize(size);
    }

    inline bool empty() const { return _pushIdx == _popIdx; }

    inline void clear()
    {
        _pushIdx = 0;
        _popIdx = 0;
    }

    inline bool full() const
    {
        return ((_pushIdx >= _popIdx) && (_pushIdx - _popIdx >= _data.size() - 1)) ||
               ((_pushIdx < _popIdx) && (_popIdx - _pushIdx <= 1));
    }

    inline bool push(const T & value)
    {
        if(full())
            return false;

        _data[_pushIdx] = value;
        if(++_pushIdx == _data.size())
            _pushIdx = 0;
        return true;
    }

    inline T pop()
    {
        T result;
        if(empty())
            return result;

        result = _data[_popIdx];
        if(++_popIdx == _data.size())
            _popIdx = 0;

        return result;
    }

    inline int available() const
    {
        if(_pushIdx >= _popIdx)
            return _pushIdx - _popIdx;
        return _pushIdx + (_data.size() - _popIdx);
    }

    inline T at(int idx) const { return _data[(_popIdx + idx) % _data.size()]; }

private:
    std::vector<T>  _data;
    int             _pushIdx;
    int             _popIdx;
};

#pragma pack(push, 1)

#pragma pack(pop)


#endif //TYPES_H
