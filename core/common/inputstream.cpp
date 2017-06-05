#include "inputstream.h"

InputStream::InputStream(Length_t num_packets) :
    _speedAdjustCoeff(0.5),
    _numPackets(num_packets),
    _maxUnderRunning(2),
    _maxOverRunning(2),
    _packetSize(0)
{
    streamReset();
}

void InputStream::initInputStream(Length_t packet_size)
{
    _packetSize = packet_size;
    _buffer.resize(_numPackets * packet_size);
}

void InputStream::endSync(Time_t time)
{
    if(!lock())
        return;

    if(time < _beginSyncTime) //! @attention timer overflow
        adjustSyncPeriod(std::numeric_limits<Time_t>::max() - _beginSyncTime + time);
    else
        adjustSyncPeriod(time - _beginSyncTime);
    _timePerPacket = _syncPeriod / _numPackets;
    unlock();
}

void InputStream::process(Time_t time)
{
    if((_syncPeriod == 0) || !lock())
        return;

    Length_t ready_count = Length_t((time - _beginSyncTime) / _timePerPacket);
    Length_t next_packet = ((_currentPacket == (_numPackets - 1)) ? 0 : (_currentPacket + 1));

    //! Under- and Over-running detection
    if(((next_packet > ready_count) && ((next_packet - _maxOverRunning) < ready_count)) ||
       ((next_packet <= ready_count) && ((next_packet + _maxUnderRunning) > ready_count)))
    {
        unlock();
        return;
    }

    //! Data processing
    streamDataReceived(_buffer.data() + _currentPacket * _packetSize, _packetSize);
    unlock();
}

void InputStream::streamReset()
{
    _currentPacket = 0;
    _beginSyncTime = 0;
    _syncPeriod = 0;
    _timePerPacket = 0;
}
