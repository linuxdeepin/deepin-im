#ifndef DIM_H
#define DIM_H

#include <QObject>
#include <QMap>

#include <memory>

class InputContext;

class Dim : public QObject {
public:
    Dim(QObject *parent = nullptr);
    ~Dim();

    uint32_t newInputContext();
    InputContext *getInputContext(uint32_t id) { return inputContexts_.value(id); }

private:
    QMap<uint32_t, InputContext *> inputContexts_;
};

#endif // !DIM_H
