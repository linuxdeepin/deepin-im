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

class InputContext;
class InputMethodAddon;
class FrontendAddon;
class Addon;
class Event;
class KeyEvent;

class Dim : public QObject
{
public:
    Dim(QObject *parent = nullptr);
    ~Dim();

    InputContext *getInputContext(uint32_t id) { return inputContexts_.value(id); }

    const QMap<QString, InputMethodEntry> &ims() const;
    const QList<QString> &enabledIMs() const;

    bool postEvent(Event &event);

private:
    QMap<uint32_t, InputContext *> inputContexts_;
    uint32_t focusedIC_;
    QMap<QString, InputMethodAddon *> inputMethodAddons_;
    QSet<FrontendAddon *> frontends_;

    QMap<QString, InputMethodEntry> ims_;
    QList<QString> enabledIMs_;

    void loadAddons();
    void loadAddon(const QString &infoFile);
    void initInputMethodAddon(InputMethodAddon *addon);
    void postInputContextCreated(Event &event);
    void postInputContextDestroyed(Event &event);
    void postInputContextFocused(Event &event);
    void postInputContextUnfocused(Event &event);
    void postKeyEvent(KeyEvent &event);
};

} // namespace dim

} // namespace deepin

} // namespace org

#endif // !DIM_H
