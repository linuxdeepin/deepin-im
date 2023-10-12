#ifndef APPMONITOR_H
#define APPMONITOR_H

#include <QObject>

namespace org::deepin::dim {

class AppMonitor : public QObject
{
    Q_OBJECT

public:
    AppMonitor();
    virtual ~AppMonitor();

signals:
    void appUpdated(const std::unordered_map<QString, pid_t> &appState, const QString &focus);
};

} // namespace org::deepin::dim

#endif // !APPMONITOR_H
