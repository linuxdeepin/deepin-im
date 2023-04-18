#include "KIMPanel2Adaptor.h"

KIMPanel2Adaptor::KIMPanel2Adaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent) {
}

void KIMPanel2Adaptor::SetSpotRect(qint32 x, qint32 y, qint32 w, qint32 h) {
    emit setSpotRect(x, y, w, h);
}

void KIMPanel2Adaptor::SetRelativeSpotRect(qint32 x, qint32 y, qint32 w, qint32 h) {
    emit setRelativeSpotRect(x, y, w, h, 1);
}

void KIMPanel2Adaptor::SetRelativeSpotRectV2(qint32 x, qint32 y, qint32 w, qint32 h, double scale) {
    emit setRelativeSpotRect(x, y, w, h, scale);
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
