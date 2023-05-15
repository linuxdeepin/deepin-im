#ifndef INPUTCONTEXTBUS_H
#define INPUTCONTEXTBUS_H

#include <QObject>

class InputcontextAdaptor;

class InputContextBus : public QObject {
    Q_OBJECT

public:
    InputContextBus(uint32_t id, QObject *parent = nullptr);
    ~InputContextBus() override;

public slots:
private:
    uint32_t id_;
    InputcontextAdaptor *adaptor_;
};

#endif // !INPUTCONTEXTBUS_H
