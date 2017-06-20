#ifndef QISTREAMCONTROLLERWRAPPER_H
#define QISTREAMCONTROLLERWRAPPER_H
#include <qnodecontrollerwrapper.h>
#include <qcore/controller/qistreamcontroller.h>

class QIStreamControllerWrapper :
        public QNodeControllerWrapper
{
    Q_OBJECT
public Q_SLOTS:
    QIStreamController * new_QIStreamController(QNodeController * node) {
        return  static_cast<QIStreamController*>(node);
    }

    bool isStreamEnabled(QIStreamController * controller) const {
        return controller->isStreamEnabled();
    }

    bool setStreamEnabled(QIStreamController * controller, bool enabled) {
        return controller->setStreamEnabled(enabled);
    }

    uint16_t numChannels(QIStreamController * controller) const {
        return controller->numChannels();
    }

    QIStreamChannelController * getChannel(QIStreamController * controller,
                                           uint16_t number) const {
        return controller->getChannel(number);
    }
};

#endif // QISTREAMCONTROLLERWRAPPER_H
