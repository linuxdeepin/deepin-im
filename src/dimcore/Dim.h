#ifndef DIM_H
#define DIM_H

#include <QObject>
#include <QMap>
#include <QSet>

#include <memory>

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

    uint32_t newInputContext();
    InputContext *getInputContext(uint32_t id) { return inputContexts_.value(id); }

    bool postEvent(Event &event);

private:
    QMap<uint32_t, InputContext *> inputContexts_;
    QMap<QString, InputMethodAddon *> inputMethodAddons_;
    QSet<FrontendAddon *> frontends_;

    void loadAddons();
    void loadAddon(const QString &infoFile);
    void postKeyEvent(KeyEvent &event);
};

#endif // !DIM_H
