#include "KIMPanel2Adaptor.h"

KIMPanel2Adaptor::KIMPanel2Adaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent) {
}

void KIMPanel2Adaptor::SetSpotRect(qint32 x, qint32 y, qint32 w, qint32 h) {
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void KIMPanel2Adaptor::SetRelativeSpotRect(qint32 x, qint32 y, qint32 w, qint32 h) {
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void KIMPanel2Adaptor::SetRelativeSpotRectV2(qint32 x, qint32 y, qint32 w, qint32 h, double scale) {
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(w);
    Q_UNUSED(h);
    Q_UNUSED(scale);
}

void KIMPanel2Adaptor::SetLookupTable(const QStringList &label,
                                      const QStringList &text,
                                      const QStringList &attr,
                                      bool hasPrev,
                                      bool hasNext,
                                      qint32 cursor,
                                      qint32 layout) {
    emit setLookupTable(label, text, attr, hasPrev, hasNext, cursor, layout);
}
