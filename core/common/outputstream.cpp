#include "outputstream.h"

OutputStream::OutputStream()
{

}

void OutputStream::initOutputStream(Length_t packet_size)
{
    _buffer.resize(packet_size);
}
