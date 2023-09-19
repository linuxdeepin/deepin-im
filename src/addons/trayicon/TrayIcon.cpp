#include "TrayIcon.h"

#include "IconProvider.h"
#include "dimcore/Dim.h"
#include "dimcore/InputContext.h"
#include "dimcore/InputMethodAddon.h"

#include <QQmlContext>

using namespace org::deepin::dim;

DIM_ADDON_FACTORY(TrayIcon)

// TODO:
static const QString DEFAULT_IM_ADDON_NAME = "none";

TrayIcon::TrayIcon(Dim *dim)
    : FrontendAddon(dim, "trayicon")
    , imAddonIcon_("org.fcitx.Fcitx5")
{
    engine.rootContext()->setContextProperty("trayicon", this);
    engine.addImageProvider(QLatin1String("icon"), new IconProvider);
    const QUrl url(QStringLiteral("qrc:/TrayIcon.qml"));
    engine.load(url);

    connect(dim, &Dim::focusedInputContextChanged, this, &TrayIcon::onFocusedInputContextChanged);
    onFocusedInputContextChanged(dim->focusedInputContext());
}

TrayIcon::~TrayIcon() { }

void TrayIcon::onFocusedInputContextChanged(int focusedInputContext)
{
    disconnect(oldConnection_);

    if (focusedInputContext == 0) {
        return;
    }

    auto *ic = dim()->getInputContext(focusedInputContext);
    oldConnection_ =
        connect(ic, &InputContext::imAddonSwitched, this, &TrayIcon::onImAddonSwitched);
}

void TrayIcon::onImAddonSwitched(const QString &imAddon)
{
    auto addon = dim()->imAddons().take(imAddon);
    imAddonIcon_ = addon->iconName();
    qDebug() << "imAddonIcon" << imAddonIcon_;
    emit imAddonIconChanged(imAddonIcon_);
}
