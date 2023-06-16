#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H

#include "Events.h"
#include "InputState.h"
#include "ObjectId.h"

#include <QObject>

namespace org {
namespace deepin {
namespace dim {

class Dim;

class InputContext : public QObject, public ObjectId<InputContext>
{
    Q_OBJECT

public:
    InputContext(Dim *dim, QObject *parent = nullptr);
    ~InputContext() = default;

public:
    void destroy();
    void focusIn();
    void focusOut();
    void keyEvent(InputContextKeyEvent &event);

    const InputState &inputState() const;

    virtual void updatePreedit(const QList<QString> &text) = 0;
    virtual void updateCommitString(const QString &text) = 0;
    virtual void forwardKey(uint32_t keyValue, uint32_t state, bool type) = 0;

private:
    Dim *dim_;
    InputState inputState_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTCONTEXT_H
