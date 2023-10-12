// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VIRTUALINPUTCONTEXTMANAGER_H
#define VIRTUALINPUTCONTEXTMANAGER_H

#include <QObject>

#include <memory>

namespace org::deepin::dim {

class AppMonitor;
class VirtualInputContext;
class Dim;
class VirtualInputContextGlue;

class VirtualInputContextManager : public QObject
{
public:
    VirtualInputContextManager(VirtualInputContextGlue *parentIC, AppMonitor *appMonitor, Dim *dim);
    ~VirtualInputContextManager();

    void setRealFocus(bool focus);
    VirtualInputContext *focusedVirtualIC();

private:
    Dim *dim_;
    VirtualInputContextGlue *parentIC_;
    AppMonitor *appMonitor_;
    std::unordered_map<QString, pid_t> lastAppState_;
    std::unordered_map<QString, std::unique_ptr<VirtualInputContext>> managed_;
    QString focus_;

    void appUpdated(const std::unordered_map<QString, pid_t> &appState, const QString &focus);
    void updateFocus();
};

} // namespace org::deepin::dim

#endif // !VIRTUALINPUTCONTEXTMANAGER_H
