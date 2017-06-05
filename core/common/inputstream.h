#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H
#include <core/common/types.h>

class InputStream
{
public:
	virtual ~InputStream() {}

    explicit InputStream(Length_t num_packets);

    void initInputStream(Length_t packet_size);

    inline void beginSync(Time_t time) { _beginSyncTime = time; }

    void endSync(Time_t time);

    void process(Time_t time);

    inline Length_t packetSize() const { return _packetSize; }

protected:
    inline Length_t streamBufferSize() const { return Length_t(_buffer.size()); }

    inline Data * streamBuffer() { return _buffer.data(); }

    void streamReset();

    virtual bool lock() = 0;

    virtual void unlock() = 0;

    virtual void streamDataReceived(const Data * data, Length_t length) = 0;

private:
    inline void adjustSyncPeriod(Time_t period)
    {
        if(_syncPeriod != 0)
            _syncPeriod += Time_t((float(_syncPeriod) - float(period)) * _speedAdjustCoeff);
        else
            _syncPeriod = period;
    }

    float       _speedAdjustCoeff;
    Length_t    _numPackets;
    Length_t    _maxUnderRunning; //! минимальное отставание ридера (выраженное в кол-ве пакетов)
    Length_t    _maxOverRunning; //! максимальное отставание ридера (выраженное в кол-ве пакетов)
    Length_t    _packetSize; //! размер пакета в байтах
    Length_t    _currentPacket; //! номер текущего пакета ридера
    Time_t      _beginSyncTime;
    Time_t      _syncPeriod;
    Time_t      _timePerPacket;
    std::vector<Data> _buffer;
};

#endif // INPUTSTREAM_H
