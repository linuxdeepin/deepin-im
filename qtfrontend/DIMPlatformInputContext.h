#ifndef DIMPLATFORMINPUTCONTEXT_H
#define DIMPLATFORMINPUTCONTEXT_H

#include <qpa/qplatforminputcontext.h>
#include <QPointer>

#include "InputmethodIface.h"
#include "InputcontextIface.h"

class QDBusPendingCallWatcher;

class DIMPlatformInputContext : public QPlatformInputContext {
    Q_OBJECT

public:
    DIMPlatformInputContext();
    ~DIMPlatformInputContext() = default;

    bool isValid() const override;
    void setFocusObject(QObject *object) override;
    void showInputPanel() override;
    void hideInputPanel() override;
    bool isInputPanelVisible() const override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    org::deepin::dim::portal::inputmethod *im_;
    org::deepin::dim::portal::inputcontext *ic_;
    QPointer<QObject> focusObject_;

private slots:
    void createIcFinished(QDBusPendingCallWatcher *);
};

#endif // !DIMPLATFORMINPUTCONTEXT_H
