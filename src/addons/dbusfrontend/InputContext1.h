#ifndef INPUTCONTEXT1_H
#define INPUTCONTEXT1_H

#include "dimcore/InputContext.h"

#include <QVariant>

class InputcontextAdaptor;

struct BatchEvent
{
    uint32_t type;
    QVariant data;
};

struct ForwardKey
{
    uint32_t keyValue;
    uint32_t state;
    bool type;

    operator QVariant() const { return QVariant::fromValue(*this); }
};

Q_DECLARE_METATYPE(BatchEvent)
Q_DECLARE_METATYPE(ForwardKey)

namespace org {
namespace deepin {
namespace dim {

class Dim;

class InputContext1 : public InputContext
{
    Q_OBJECT

public:
    InputContext1(Dim *dim, QObject *parent = nullptr);
    ~InputContext1() override;

    const QString path() { return path_; }

    void updatePreedit(const QList<QString> &preedit) override;
    void updateCommitString(const QString &text) override;
    void forwardKey(uint32_t keyValue, uint32_t state, bool type) override;

public slots:
    void FocusIn();
    void FocusOut();
    void Destroy();
    QList<BatchEvent> ProcessKeyEvent(
        uint32_t keyval, uint32_t keycode, uint32_t state, bool isRelease, uint32_t time);

private:
    InputcontextAdaptor *adaptor_;
    QString path_;

    QList<BatchEvent> blockedEvents_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTCONTEXT1_H
