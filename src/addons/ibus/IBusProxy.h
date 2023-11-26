// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IBUSPROXY_H
#define IBUSPROXY_H

#include "IBUSInputContextIface.h"
#include "common/common.h"

#include <dimcore/ProxyAddon.h>
#include <gio/gio.h>

#include <QProcess>

#include <memory>

class IBusEngineDesc;
class DimIBusInputContextPrivate;

namespace org {
namespace deepin {
namespace dim {

class DimIBusProxy : public ProxyAddon
{
public:
    explicit DimIBusProxy(Dim *dim);
    ~DimIBusProxy() override;

    const QList<InputMethodEntry> &getInputMethods() override;
    void initInputMethods() override;
    bool keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent) override;

    void focusIn(uint32_t id) override;
    void focusOut(uint32_t id) override;
    void destroyed(uint32_t id) override;
    void done() override;
    void contentType(uint32_t hint, uint32_t purpose) override;
    void updateSurroundingText(InputContextEvent &event) override;
    void cursorRectangleChangeEvent(InputContextCursorRectChangeEvent &event) override;
    void setCurrentIM(const std::string &im) override;

private:
    QList<IBusEngineDesc> listEngines();
    void initEngines();

    inline bool isICDBusInterfaceValid(uint32_t id)
    {
        return !iBusICMap_.isEmpty() && iBusICMap_.contains(id) && iBusICMap_[id]->isValid();
    }
    void launchDaemon();

public Q_SLOTS:
    void connectToBus();
    void socketChanged(const QString &str);
    void busRegistered(const QString &str);
    void busUnregistered(const QString &str);
    void globalEngineChanged(const QString &engineName);

private:
    bool shouldBeIgnored(const std::string &uniqueName) const;
    InputContext *isValidIC(uint32_t id) const;

private:
    std::unique_ptr<GSettings, Deleter<g_object_unref>> gsettings_;
    std::unique_ptr<DimIBusInputContextPrivate> d;
    bool useSyncMode_;
    QFileSystemWatcher socketWatcher_;
    QTimer timer_;
    QList<InputMethodEntry> inputMethods_;
    QMap<uint32_t, std::shared_ptr<OrgFreedesktopIBusInputContextInterface>> iBusICMap_;
    QProcess *ibusDaemonProc_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // IBUSPROXY_H