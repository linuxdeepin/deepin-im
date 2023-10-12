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
class InputState;
class InputMethodAddon;
class ProxyAddon;
class FrontendAddon;
class Addon;
class Event;
class InputContextKeyEvent;
class InputContextCursorRectChangeEvent;
class InputContextSetSurroundingTextEvent;

class Dim : public QObject
{
    Q_OBJECT

public:
    explicit Dim(QObject *parent = nullptr);
    ~Dim();

    QList<QString> imAddonNames() const;
    bool postEvent(Event &event);

    QMap<uint32_t, InputContext *> getInputContexts() const { return inputContexts_; }

    InputContext *getInputContext(uint32_t id) const { return inputContexts_.value(id); }

    QMap<QString, InputMethodAddon *> imAddons() const { return inputMethodAddons_; }

    std::vector<InputMethodEntry> imEntries() const { return imEntries_; };

    int focusedInputContext() const { return focusedInputContext_; }

Q_SIGNALS:
    void focusedInputContextChanged(int focusedInputContext);
    void inputMethodEntryChanged();

public Q_SLOTS:
    void switchIM(const QPair<QString, QString> &imIndex);

private:
    void loadAddons();
    void loadAddon(const QString &infoFile);
    void initInputMethodAddon(InputMethodAddon *imAddon);
    void postInputContextCreated(Event &event);
    void postInputContextDestroyed(Event &event);
    void postInputContextFocused(Event &event);
    void postInputContextUnfocused(Event &event);
    bool postInputContextKeyEvent(InputContextKeyEvent &event);
    void postInputContextCursorRectChanged(InputContextCursorRectChangeEvent &event);
    void postInputContextSetSurroundingTextEvent(Event &event);
    InputMethodAddon *getInputMethodAddon(const InputState &state);

    template<typename T>
    T getImAddon(InputMethodAddon *imAddon) const;

private:
    QMap<uint32_t, InputContext *> inputContexts_;
    uint32_t focusedInputContext_;
    QMap<QString, InputMethodAddon *> inputMethodAddons_;
    QSet<FrontendAddon *> frontends_;
    std::vector<InputMethodEntry> imEntries_;
};

} // namespace dim

} // namespace deepin

} // namespace org

#endif // !DIM_H
