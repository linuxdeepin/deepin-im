#ifndef KIMPANEL2ADAPTOR_H
#define KIMPANEL2ADAPTOR_H

#include <QDBusAbstractAdaptor>

class KIMPanel2Adaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.impanel2")

public:
    explicit KIMPanel2Adaptor(QObject *parent = nullptr);

public slots:
    void SetSpotRect(qint32 x, qint32 y, qint32 w, qint32 h);
    void SetRelativeSpotRect(qint32 x, qint32 y, qint32 w, qint32 h);
    void SetRelativeSpotRectV2(qint32 x, qint32 y, qint32 w, qint32 h, double scale);
    void SetLookupTable(const QStringList &label,
                        const QStringList &text,
                        const QStringList &attr,
                        bool hasPrev,
                        bool hasNext,
                        qint32 cursor,
                        qint32 layout);

signals:
    void setLookupTable(const QStringList &label,
                        const QStringList &text,
                        const QStringList &attr,
                        bool hasPrev,
                        bool hasNext,
                        qint32 cursor,
                        qint32 layout);

signals:
    void PanelCreated2();
};

#endif // !KIMPANEL2ADAPTOR_H
