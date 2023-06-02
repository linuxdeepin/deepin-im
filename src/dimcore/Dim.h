#ifndef DIM_H
#define DIM_H

#include <QObject>
#include <QMap>
#include <QSet>
#include <QList>

#include "InputMethodEntry.h"

class InputContext;
class InputMethodAddon;
class FrontendAddon;
class Addon;
class Event;
class KeyEvent;

class Dim : public QObject {
public:
    Dim(QObject *parent = nullptr);
    ~Dim();

    void inputContextCreated(InputContext *ic);
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
    void postKeyEvent(KeyEvent &event);
};

#endif // !DIM_H
