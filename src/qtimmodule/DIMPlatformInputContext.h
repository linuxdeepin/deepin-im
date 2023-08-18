// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DIMPLATFORMINPUTCONTEXT_H
#define DIMPLATFORMINPUTCONTEXT_H

#include <qpa/qplatforminputcontext.h>

#include <QPointer>

class QDBusPendingCallWatcher;
class DimTextInputV1;

class DIMPlatformInputContext : public QPlatformInputContext
{
    Q_OBJECT

public:
    DIMPlatformInputContext();
    ~DIMPlatformInputContext() = default;

    bool isValid() const override;

    void reset() override;

    void setFocusObject(QObject *object) override;
    void showInputPanel() override;
    void hideInputPanel() override;
    bool isInputPanelVisible() const override;

    bool filterEvent(const QEvent *event) override;

public slots:
    void preedit(const QList<QString> &preedit);
    void commitString(const QString &text);
    void forwardKey(uint32_t keyValue, uint32_t state, bool type);

private:
    DimTextInputV1 *proxy_;
    QPointer<QObject> focusObject_;

private slots:
};

#endif // !DIMPLATFORMINPUTCONTEXT_H
