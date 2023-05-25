#ifndef DIMPLATFORMINPUTCONTEXT_H
#define DIMPLATFORMINPUTCONTEXT_H

#include <qpa/qplatforminputcontext.h>
#include <QPointer>

class QDBusPendingCallWatcher;
class InputContextProxy;

class DIMPlatformInputContext : public QPlatformInputContext {
    Q_OBJECT

public:
    DIMPlatformInputContext();
    ~DIMPlatformInputContext() = default;

    bool isValid() const override;
    void update(Qt::InputMethodQueries queries) override;
    void setFocusObject(QObject *object) override;
    void showInputPanel() override;
    void hideInputPanel() override;
    bool isInputPanelVisible() const override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    InputContextProxy *proxy_;
    QPointer<QObject> focusObject_;

private slots:
};

#endif // !DIMPLATFORMINPUTCONTEXT_H
