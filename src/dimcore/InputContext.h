#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H

#include <QObject>
#include "ObjectId.h"
#include "Events.h"

class Dim;
class InputMethodEntry;

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

    InputMethodEntry *currentIM();

signals:
    void focused();
    void unFocused();

private:
    Dim *dim_;
    InputMethodEntry *current_im_;
};

#endif // !INPUTCONTEXT_H
