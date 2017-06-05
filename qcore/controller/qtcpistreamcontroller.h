#ifndef QTCPISTREAMCONTROLLER_H
#define QTCPISTREAMCONTROLLER_H
#include <controller/qistreamcontroller.h>
#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>

class QTcpIStreamController :
        public QIStreamController
{
    Q_OBJECT
public:
    QTcpIStreamController(NodeID_t id = NodeIDNull,
                          NodeID_t parent_id = NodeIDNull,
                          const QString & name = QString(),
                          QDeviceController * device = nullptr,
                          QObject * parent = nullptr);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceController * device) override;

    virtual NodeControllerForm * createForm(QWidget * parent) override;

protected:
    virtual bool parseCustomDescriptor(const ControlPacket & packet) override;

    virtual void statusChanged(NodeStatus_t status) override {} //! @todo

    virtual void streamToggled(bool enabled) override;

    virtual bool init() override;

    virtual void sendStreamData(const QByteArray & data) override;

private:
    QTcpSocket *    _socket;
    QHostAddress    _hostAddress;
    quint16         _port;

private slots:
    void socketDisconnected();
};

#endif // QTCPISTREAMCONTROLLER_H
