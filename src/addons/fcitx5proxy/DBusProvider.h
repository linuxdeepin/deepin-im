// SPDX-FileCopyrightText: 2020~2020 CSSlayer <wengxt@gmail.com>
// SPDX-FileCopyrightText: 2023 Deepin Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DBUSPROVIDER_H
#define DBUSPROVIDER_H

#include <fcitxqtcontrollerproxy.h>
#include <fcitxqtinputmethodproxy.h>
#include <fcitxqtwatcher.h>

#include <QObject>

using namespace fcitx;

namespace org {
namespace deepin {
namespace dim {
class DBusProvider : public QObject
{
    Q_OBJECT

public:
    DBusProvider(QObject *parent = nullptr);
    ~DBusProvider();

    inline bool available() const { return controller_ != nullptr; }

    inline FcitxQtControllerProxy *controller() { return controller_; }

    inline FcitxQtInputMethodProxy *imProxy() { return imProxy_; }

signals:
    void availabilityChanged(bool avail);

private slots:
    void fcitxAvailabilityChanged(bool avail);

private:
    FcitxQtWatcher *watcher_;
    FcitxQtControllerProxy *controller_ = nullptr;
    FcitxQtInputMethodProxy *imProxy_ = nullptr;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif
