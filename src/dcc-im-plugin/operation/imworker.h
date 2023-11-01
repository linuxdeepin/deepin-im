// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IMWORKER_H
#define IMWORKER_H

#include "keyvalue.h"
#include "operation/configoption.h"
#include "operation/immodel.h"
#include "operation/inputmethodentry.h"

#include <dstyleoption.h>
#include <dtkwidget_global.h>
#include <qlist.h>
#include <qmap.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpair.h>
#include <qt6/QtCore/qtmetamacros.h>
#include <qvariant.h>

#include <DDBusInterface>

QString languageName(const QString &uniqueName, const QString &langCode);

using Dtk::ItemDataRole;
using Dtk::Core::DDBusInterface;

namespace dcc_im_plugin {
namespace operation {

enum {
    RowTypeRole = Dtk::ItemDataRole::UserRole + 1,
    LanguageRole,
    LanguageNameRole,
    IMUniqueNameRole,
    IMConfigurableRole,
    IMLayoutRole,
    IMActiveRole,

    IMEnabledRole,
};
enum class CONTAIN_CUR_LANG {
    NO,
    YES,
    DIFF_VARIANT,
};

enum { LanguageType, IMType };

class IMWorker : public QObject
{
    Q_OBJECT
public:
    static IMWorker *instance();

    inline InputMethodEntryList allIMEntries() const { return m_allIMEntries; }

    inline QMap<QString, InputMethodEntryList> availableIMEntries() const
    {
        return m_availableIMEntryList;
    }

    void setInitializedStatus(const bool initialized);

    inline QString currentGroup() const { return m_currentGroup; }

    void fetchConfig(const QString &uri);
    void fetchAvailableInputMethodEntries();
    void fetchGroups();
    void fetchCurrentGroupInfo(const QString &group);

    void updateEnabledIM(const InputMethodEntryList &imEntryList,
                         const KeyValueList &enabledIMList);
    void launchStore();

    inline int enabledLanguageCount() { return m_enabledLanguageCount; }

    inline QStringList enabledLanguages() { return m_enabledLanguages; }

Q_SIGNALS:
    void
    fetchAvailableIMEntriesFinished(const QMap<QString, InputMethodEntryList> &availableIMEntries);
    void fetchConfigDescriptionFinished(const QMap<QString, ConfigOptionList> &description,
                                        const QString &mainType);
    void fetchConfigValueFinished(const QVariant &variant);
    void fetchAvailableInputMethodEntriesFinished(const InputMethodEntryList &imEntryList);
    void fetchGroupsFinished(const QStringList &groups);
    void fetchGroupInfoFinished(const KeyValueList &infos);
    void currentGroupChanged(const QString &currentGroup);
    void enabledIMEntriesChanged(const InputMethodEntryList &enabledIMEntries);

private:
    IMWorker(QObject *parent = nullptr);
    bool compare(const QString &left, const QString &right);
    QPair<bool, CONTAIN_CUR_LANG> checkCategories(const QString &key);

private:
    DDBusInterface *m_interface;
    bool m_initialized;
    QStringList m_groups;
    QString m_currentGroup;
    QString m_defaultLayout;
    KeyValueList m_imEntries;
    InputMethodEntryList m_allIMEntries;
    InputMethodEntryList m_enabledIMEntries;
    QMap<QString, InputMethodEntryList> m_availableIMEntryList;
    QStringList m_enabledLanguages;
    static int m_enabledLanguageCount;
    /* QSet<QString> m_enabledIMs; */
    /* QMap<QString, ConfigOptionList> m_desc; */
    /* QString m_mainType; */
};
} // namespace operation
} // namespace dcc_im_plugin

#endif // !IMWORKER_H
