#ifndef QOSTREAMCONTROLLERWRAPPER_H
#define QOSTREAMCONTROLLERWRAPPER_H
#include <qcore/controller/qostreamcontroller.h>
#include <qnodecontrollerwrapper.h>

class QOStreamControllerWrapper :
        public QNodeControllerWrapper
{
    Q_OBJECT
public Q_SLOTS:
    QOStreamController * new_QOStreamController(QNodeController * node) {
        return static_cast<QOStreamController*>(node);
    }

    bool isStreamEnabled(QOStreamController * controller) const {
        return controller->isStreamEnabled();
    }

    bool setStreamEnabled(QOStreamController * controller, bool enabled) {
        return controller->setStreamEnabled(enabled);
    }

    uint16_t numChannels(QOStreamController * controller) const {
        return controller->numChannels();
    }

    QOStreamChannelController * getChannel(QOStreamController * controller,
                                           uint16_t number) const {
        return controller->getChannel(number);
    }
};

#endif // QOSTREAMCONTROLLERWRAPPER_H
