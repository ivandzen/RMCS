#ifndef QOPARAMCONTROLLER_H
#define QOPARAMCONTROLLER_H
#include <qcore/controller/qostreamchannelcontroller.h>
#include <functional>
#include <QVariant>
#include <QQueue>
#include <QMutex>
#include <QVector>

////////////////////////////////////////////////////////////////////////////////////////////////

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
    QVector<T>  _data;
    int         _pushIdx;
    int         _popIdx;
};

////////////////////////////////////////////////////////////////////////////////////////////////

class QOParamController :
        public QOStreamChannelController
{
    Q_OBJECT
public :
    typedef enum
    {
        STATE_STOPPED,
        STATE_RUNNING,
        STATE_BUF_FULL
    }
    State;

    QOParamController(NodeType_t type,
                      NodeID_t node_id = NodeIDNull,
                      NodeID_t parent_id = NodeIDNull,
                      const QString & name = QString(),
                      QDeviceConnection * dev = nullptr);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) override;

    inline State state() const { return _state; }

    inline int bufferSize()
    {
        QMutexLocker locker(&_mutex);
        return _buffer.size();
    }

    inline void setBufferSize(uint32_t size)
    {
        QMutexLocker locker(&_mutex);
        _buffer.resize(size);
    }

    virtual NodeType_t type() const override { return _type; }

    inline void reset()
    {
        QMutexLocker locker(&_mutex);
        _buffer.clear();
        _state = STATE_RUNNING;
    }

    inline void pause() { _state = STATE_STOPPED; }

    inline bool empty()
    {
        QMutexLocker locker(&_mutex);
        return _buffer.empty();
    }

    inline QVariant popValue()
    {
        QMutexLocker locker(&_mutex);
        if(_buffer.empty())
            return QVariant();
        return _buffer.pop();
    }

    inline void map(const std::function<void(const QVariant &)> & handler)
    {
        QMutexLocker locker(&_mutex);
        for(int i = 0; i < _buffer.available(); ++i)
            handler(_buffer.at(i));
    }

signals :
    void bufferFull();

protected:
    virtual void streamDataReceived(Data * data) override;

private:
    QMutex      _mutex;
    NodeType_t  _type;
    uint8_t     _lastId;
    FIFO<QVariant> _buffer;
    State       _state;
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif // QOPARAMCONTROLLER_H
