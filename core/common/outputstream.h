#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H
#include <core/common/types.h>

class OutputStream
{
public:
    OutputStream();

    void initOutputStream(Length_t packet_size);

    inline Length_t packetSize() const { return Length_t(_buffer.size()); }

    inline DataPtr streamBuffer() { return _buffer.data(); }

private:
    std::vector<Data> _buffer;
};

#endif // OUTPUTSTREAM_H
