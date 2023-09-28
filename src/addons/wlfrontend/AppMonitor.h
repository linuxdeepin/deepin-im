#ifndef APPMONITOR_H
#define APPMONITOR_H

#include <QObject>

class AppMonitor : public QObject
{
    Q_OBJECT

public:
    AppMonitor();
    virtual ~AppMonitor();

signals:
    void appUpdated(const std::unordered_map<QString, pid_t> &appState, const QString &focus);
};

#endif // !APPMONITOR_H
