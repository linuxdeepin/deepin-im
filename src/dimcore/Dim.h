#ifndef DIM_H
#define DIM_H

#include <QObject>
#include <QMap>
#include <QSet>

#include <memory>

class InputContext;
class Addon;

class Dim : public QObject {
public:
    Dim(QObject *parent = nullptr);
    ~Dim();

    uint32_t newInputContext();
    InputContext *getInputContext(uint32_t id) { return inputContexts_.value(id); }

private:
    QMap<uint32_t, InputContext *> inputContexts_;
    QSet<Addon *> addons_;

    void loadAddons();
    void loadAddon(const QString &infoFile);
};

#endif // !DIM_H
