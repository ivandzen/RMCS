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

    uint16_t numChannels(QOStreamController * controller) const {
        return controller->numChannels();
    }

    QOStreamChannelController * getChannel(QOStreamController * controller,
                                           uint16_t number) const {
        return controller->getChannel(number);
    }

    bool setBufCapacity(QOStreamController * controller,
                        Length_t cap) {
        return controller->setBufCapacity(cap);
    }

    bool setPPT(QOStreamController * controller,
                Length_t ppt) {
        return controller->setPPT(ppt);
    }
};

#endif // QOSTREAMCONTROLLERWRAPPER_H
