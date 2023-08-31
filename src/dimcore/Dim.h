// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DIM_H
#define DIM_H

#include "InputMethodEntry.h"

#include <QList>
#include <QMap>
#include <QObject>
#include <QSet>

namespace org {
namespace deepin {
namespace dim {

enum AddonType {
    Frontend,
    InputMethod,
    Proxy,
};

class InputContext;
class InputMethodAddon;
class ProxyAddon;
class FrontendAddon;
class Addon;
class Event;
class InputContextKeyEvent;

class Dim : public QObject
{
    Q_OBJECT
public:
    Dim(QObject *parent = nullptr);
    ~Dim();

    InputContext *getInputContext(uint32_t id) { return inputContexts_.value(id); }

    QList<QString> imAddons() const;

    bool postEvent(Event &event);

    ProxyAddon *getCurrentImAddon();

Q_SIGNALS:
    void imChanged();

private:
    QMap<uint32_t, InputContext *> inputContexts_;
    uint32_t focusedIC_;
    InputMethodAddon *currentImAddon;
    QMap<QString, InputMethodAddon *> inputMethodAddons_;
    QSet<FrontendAddon *> frontends_;

    QMap<QString, InputMethodEntry> ims_;

    void loadAddons();
    void loadAddon(const QString &infoFile);
    void initInputMethodAddon(InputMethodAddon *imAddon);
    void postInputContextCreated(Event &event);
    void postInputContextDestroyed(Event &event);
    void postInputContextFocused(Event &event);
    void postInputContextUnfocused(Event &event);
    bool postInputContextKeyEvent(InputContextKeyEvent &event);
};

} // namespace dim

} // namespace deepin

} // namespace org

#endif // !DIM_H
