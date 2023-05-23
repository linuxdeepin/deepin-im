#ifndef INPUTCONTEXT1_H
#define INPUTCONTEXT1_H

#include "dimcore/InputContext.h"

class InputcontextAdaptor;

class InputContext1 : public InputContext {
    Q_OBJECT

public:
    InputContext1(QObject *parent = nullptr);
    ~InputContext1() override;

    const QString path() { return path_; }

public slots:
    void FocusIn();
    void FocusOut();
    void Destroy();
    void ProcessKeyEvent(uint32_t keyval, uint32_t keycode, uint32_t state, bool isRelease, uint32_t time);

private:
    InputcontextAdaptor *adaptor_;
    QString path_;
};

#endif // !INPUTCONTEXT1_H
