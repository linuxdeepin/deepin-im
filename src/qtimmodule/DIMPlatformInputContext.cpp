// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DIMPlatformInputContext.h"

#include "DimTextInputV1.h"

#include <QDBusConnection>
#include <QDebug>
#include <QEvent>
#include <QGuiApplication>
#include <QKeyEvent>

static const QString DIM_SERVICE = "org.deepin.dim";
static const QString DIM_IM_PATH = "/org/freedesktop/portal/inputmethod";

DIMPlatformInputContext::DIMPlatformInputContext()
    : QPlatformInputContext()
    , proxy_(new DimTextInputV1(this))
    , focusObject_(nullptr)
{
    connect(proxy_, &DimTextInputV1::preedit, this, &DIMPlatformInputContext::preedit);
    connect(proxy_, &DimTextInputV1::commitString, this, &DIMPlatformInputContext::commitString);
    connect(proxy_, &DimTextInputV1::forwardKey, this, &DIMPlatformInputContext::forwardKey);
}

bool DIMPlatformInputContext::isValid() const
{
    return true;
}

void DIMPlatformInputContext::reset() {
    QPlatformInputContext::reset();
}

void DIMPlatformInputContext::setFocusObject(QObject *object)
{
    qDebug() << "setFocusObject";
    if (focusObject_ == object) {
        return;
    }

    if (focusObject_) {
        proxy_->focusOut();
        focusObject_ = nullptr;
    }

    if (!object) {
        return;
    }

    QInputMethodQueryEvent qe(Qt::ImQueryInput | Qt::ImEnabled);
    QGuiApplication::sendEvent(object, &qe);

    if (!qe.value(Qt::ImEnabled).toBool()) {
        return;
    }

    focusObject_ = object;
    proxy_->focusIn();
}

void DIMPlatformInputContext::showInputPanel()
{
    qDebug() << "showInputPanel";
}

void DIMPlatformInputContext::hideInputPanel()
{
    qDebug() << "hideInputPanel";
}

bool DIMPlatformInputContext::isInputPanelVisible() const
{
    qDebug() << "isInputPanelVisible";
    return true;
}

bool DIMPlatformInputContext::filterEvent(const QEvent *event)
{
    if (event->type() != QEvent::KeyPress && event->type() != QEvent::KeyRelease) {
        return false;
    }

    if (!proxy_->available()) {
        return false;
    }

    return true;
}

void DIMPlatformInputContext::preedit(const QList<QString> &preedit)
{
    QInputMethodEvent qe(preedit.join(""), {});
    QGuiApplication::sendEvent(focusObject_, &qe);
}

void DIMPlatformInputContext::commitString(const QString &text)
{
    QInputMethodEvent qe;
    qe.setCommitString(text);
    QGuiApplication::sendEvent(focusObject_, &qe);
}

void DIMPlatformInputContext::forwardKey(uint32_t keyValue, uint32_t state, bool type)
{
    Q_UNUSED(keyValue);
    Q_UNUSED(state);
    Q_UNUSED(type);
}
