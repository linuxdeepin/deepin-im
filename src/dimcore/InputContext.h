#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H

#include <QObject>

#include "ObjectId.h"
#include "Events.h"
#include "InputState.h"

class Dim;

class InputContext : public QObject, public ObjectId<InputContext> {
    Q_OBJECT

public:
    InputContext(Dim *dim, QObject *parent = nullptr);
    ~InputContext() = default;

public:
    void destroy();
    void focusIn();
    void focusOut();
    void keyEvent(KeyEvent &event);

    const InputState &inputState() const;

signals:
    void focused();
    void unFocused();

private:
    Dim *dim_;
    InputState inputState_;
};

#endif // !INPUTCONTEXT_H
