#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H

#include <QObject>
#include "ObjectId.h"
#include "Events.h"

class InputContext : public QObject, public ObjectId<InputContext> {
    Q_OBJECT

public:
    InputContext(QObject *parent = nullptr);
    ~InputContext() = default;

public:
    void destroy();
    void focusIn();
    void focusOut();
    void keyEvent(KeyEvent &event);

signals:
    void focused();
    void unFocused();
};

#endif // !INPUTCONTEXT_H
