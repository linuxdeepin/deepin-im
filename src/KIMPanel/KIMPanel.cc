#include "KIMPanel.h"

#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QTransform>

#include "KIMPanelAdaptor.h"
#include "KIMPanel2Adaptor.h"

using namespace org::kde::kimpanel;

static const QString BUS_SERVICE = QStringLiteral("org.kde.impanel");
static const QString BUS_PATH = QStringLiteral("/org/kde/impanel");

static const QString INPUTMETHOD_BUS_SERVICE = QStringLiteral("org.kde.kimpanel.inputmethod");
static const QString INPUTMETHOD_BUS_PATH = QStringLiteral("/kimpanel");
static const QString INPUTMETHOD_BUS_INTERFACE = QStringLiteral("org.kde.kimpanel.inputmethod");

static const QVariantMap EMPTY_ELEMENT{{"text", ""}, {"attr", ""}};

KIMPanel::KIMPanel(QObject *parent)
    : QObject(parent)
    , bus_(QDBusConnection::sessionBus())
    , watcher_(new QDBusServiceWatcher(INPUTMETHOD_BUS_SERVICE, bus_, QDBusServiceWatcher::WatchForOwnerChange, this))
    , inputmethodIface_(new org::kde::kimpanel::inputmethod(INPUTMETHOD_BUS_SERVICE, INPUTMETHOD_BUS_PATH, bus_, this))
    , kimpanelAdaptor_(new KIMPanelAdaptor(this))
    , kimpanel2Adaptor_(new KIMPanel2Adaptor(this))
    , enable_(false)
    , showAux_(false)
    , showLookupTable_(false)
    , showPreedit_(false)
    , aux_(EMPTY_ELEMENT)
    , preedit_(EMPTY_ELEMENT) {
    bus_.registerService(BUS_SERVICE);

    if (!bus_.registerObject(BUS_PATH, this)) {
        qWarning() << "Failed to register request object for" << bus_.lastError().message();
    }

    connect(watcher_, &QDBusServiceWatcher::serviceOwnerChanged, this, &KIMPanel::onServiceOwnerChanged);

    connect(inputmethodIface_, &inputmethod::Enable, this, &KIMPanel::onEnable);
    connect(inputmethodIface_, &inputmethod::ExecDialog, this, &KIMPanel::onExecDialog);
    connect(inputmethodIface_, &inputmethod::ExecMenu, this, &KIMPanel::onExecMenu);
    connect(inputmethodIface_, &inputmethod::RegisterProperties, this, &KIMPanel::onRegisterProperties);
    connect(inputmethodIface_, &inputmethod::RemoveProperty, this, &KIMPanel::onRemoveProperty);
    connect(inputmethodIface_, &inputmethod::ShowAux, this, &KIMPanel::onShowAux);
    connect(inputmethodIface_, &inputmethod::ShowLookupTable, this, &KIMPanel::onShowLookupTable);
    connect(inputmethodIface_, &inputmethod::ShowPreedit, this, &KIMPanel::onShowPreedit);
    connect(inputmethodIface_, &inputmethod::UpdateAux, this, &KIMPanel::onUpdateAux);
    connect(inputmethodIface_, &inputmethod::UpdateLookupTable, this, &KIMPanel::onUpdateLookupTable);
    connect(inputmethodIface_, &inputmethod::UpdateLookupTableCursor, this, &KIMPanel::onUpdateLookupTableCursor);
    connect(inputmethodIface_, &inputmethod::UpdatePreeditCaret, this, &KIMPanel::onUpdatePreeditCaret);
    connect(inputmethodIface_, &inputmethod::UpdatePreeditText, this, &KIMPanel::onUpdatePreeditText);
    connect(inputmethodIface_, &inputmethod::UpdateProperty, this, &KIMPanel::onUpdateProperty);
    connect(inputmethodIface_, &inputmethod::UpdateSpotLocation, this, &KIMPanel::onUpdateSpotLocation);

    connect(kimpanel2Adaptor_, &KIMPanel2Adaptor::setSpotRect, this, &KIMPanel::onSetSpotRect);
    connect(kimpanel2Adaptor_, &KIMPanel2Adaptor::setRelativeSpotRect, this, &KIMPanel::onSetRelativeSpotRect);
    connect(kimpanel2Adaptor_, &KIMPanel2Adaptor::setLookupTable, this, &KIMPanel::onSetLookupTable);
}

void KIMPanel::onServiceOwnerChanged([[maybe_unused]] const QString &service,
                                     [[maybe_unused]] const QString &oldOwner,
                                     const QString &newOwner) {
    if (newOwner.isEmpty()) {
        // TODO:
        return;
    }

    // TODO:
}

void KIMPanel::onEnable(bool enable) {
    enable_ = enable;
    emit enableChanged(enable_);
}

void KIMPanel::onExecDialog(const QString &prop) {
    Q_UNUSED(prop);
}

void KIMPanel::onExecMenu(const QStringList &actions) {
    Q_UNUSED(actions);
}

void KIMPanel::onRegisterProperties(const QStringList &prop) {
    Q_UNUSED(prop);
}

void KIMPanel::onRemoveProperty(const QString &prop) {
    Q_UNUSED(prop);
}

void KIMPanel::onShowAux(bool toshow) {
    qInfo() << "onShowAux" << toshow;
    showAux_ = toshow;
    emit showAuxChanged(showAux_);
}

void KIMPanel::onShowLookupTable(bool toshow) {
    qInfo() << "onShowLookupTable" << toshow;
    showLookupTable_ = toshow;
    emit showLookupTableChanged(showLookupTable_);
}

void KIMPanel::onShowPreedit(bool toshow) {
    qInfo() << "onShowPreedit" << toshow;
    showPreedit_ = toshow;
    emit showPreeditChanged(showPreedit_);
}

void KIMPanel::onUpdateAux(const QString &text, const QString &attr) {
    aux_ = QVariantMap{{"text", text}, {"attr", attr}};
    emit auxChanged(aux_);
}

void KIMPanel::onUpdateLookupTable(const QStringList &labels,
                                   const QStringList &candidates,
                                   const QStringList &attrs,
                                   bool hasprev,
                                   bool hasnext) {
    Q_UNUSED(labels);
    Q_UNUSED(candidates);
    Q_UNUSED(attrs);
    Q_UNUSED(hasprev);
    Q_UNUSED(hasnext);
}

void KIMPanel::onUpdateLookupTableCursor(int pos) {
    Q_UNUSED(pos);
}

void KIMPanel::onUpdatePreeditCaret(int pos) {
    preeditCaretPos_ = pos;
    emit preeditCaretPosChanged(preeditCaretPos_);
}

void KIMPanel::onUpdatePreeditText(const QString &text, const QString &attr) {
    preedit_ = QVariantMap{{"text", text}, {"attr", attr}};
    emit preeditChanged(preedit_);
}

void KIMPanel::onUpdateProperty(const QString &prop) {
    Q_UNUSED(prop);
}

void KIMPanel::onUpdateSpotLocation(int x, int y) {
    qInfo() << "onUpdateSpotLocation" << QPoint(x, y);
}

void KIMPanel::onSetLookupTable(const QStringList &label,
                                const QStringList &text,
                                const QStringList &attr,
                                bool hasPrev,
                                bool hasNext,
                                qint32 cursor,
                                qint32 layout) {
    hasPrev_ = hasPrev;
    hasNext_ = hasNext;
    cursor_ = cursor;
    layout_ = layout;

    QVariantList list;
    list.reserve(label.size());
    for (int i = 0; i < label.size(); i++) {
        list << QVariant{QVariantMap{
            {"label", label[i]},
            {"text", text[i]},
            {"attr", attr[i]},
        }};
    }
    lookupTable_.swap(list);

    emit lookupTableChanged();
}

void KIMPanel::onSetSpotRect(qint32 x, qint32 y, qint32 w, qint32 h) {
    QPoint ori(x + w, y + h);
    QTransform trans;
    trans /= 1.25;
    pos_ = trans.map(ori);

    emit posChanged(pos_);
}

void KIMPanel::onSetRelativeSpotRect(qint32 x, qint32 y, qint32 w, qint32 h, double scale) {
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(w);
    Q_UNUSED(h);
    Q_UNUSED(scale);
}
