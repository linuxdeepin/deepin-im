#ifndef DIMPLATFORMINPUTCONTEXT_H
#define DIMPLATFORMINPUTCONTEXT_H

#include <qpa/qplatforminputcontext.h>
#include <QPointer>

class DIMPlatformInputContext : public QPlatformInputContext {
    Q_OBJECT

public:
    bool isValid() const override;
    void setFocusObject(QObject *object) override;
    void showInputPanel() override;
    void hideInputPanel() override;
    bool isInputPanelVisible() const override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QPointer<QObject> focusObject_;
};

#endif // !DIMPLATFORMINPUTCONTEXT_H
