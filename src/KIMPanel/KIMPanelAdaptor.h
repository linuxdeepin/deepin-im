#ifndef KIMPANELADAPTOR_H
#define KIMPANELADAPTOR_H

#include <QDBusAbstractAdaptor>

class KIMPanelAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.impanel")

public:
    explicit KIMPanelAdaptor(QObject *parent = nullptr);

signals:
    void MovePreeditCaret(qint32 potision);
    void SelectCandidate(qint32 index);
    void LookupTablePageUp();
    void LookupTablePageDown();
    void TriggerProperty(const QString &key);
    void PanelCreated();
    void Exit();
    void ReloadConfig();
    void Configure();
};

#endif // !KIMPANELADAPTOR_H
