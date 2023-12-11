// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "imworker.h"

#include "configtype.h"
#include "inputmethodentry.h"
#include "operation/keyvalue.h"
#include "utils/translate.h"
#include "xkbrules.h"

#include <qdbuspendingcall.h>
#include <qglobal.h>
#include <qlocale.h>
#include <qobject.h>
#include <qpair.h>

#include <DDBusSender>

#include <QCollator>
#include <QDBusMetaType>
#include <QDBusPendingReply>
#include <QDebug>

static const QString server = "org.fcitx.Fcitx5";
static const QString path = "/controller";
static const QString interface = "org.fcitx.Fcitx.Controller1";

using namespace dcc_im_plugin::operation;

QString languageName(const QString &uniqueName, const QString &langCode)
{
    const auto &xkbrules = XkbRules::instance();
    QString langName;

    if (uniqueName.startsWith("keyboard-")) {
        auto layout = xkbrules.layout(uniqueName);
        langName = layout.description;
    }

    if (!langName.isEmpty()) {
        return XkbRules::tr(langName);
    }

    if (langCode.isEmpty()) {
        return _("Unknown");
    } else if (langCode == "*") {
        return _("Multilingual");
    }

    langName = QLocale(langCode).nativeLanguageName();
    if (langName.isEmpty()) {
        langName = "Unknown";
        qInfo("NOTICE: uniqueName [%s] not found english name. unknown.",
              uniqueName.toStdString().c_str());
    }

    return langName;
}

static QString getEnglishLanguageName(const QString &uniqueName, const QString &langCode)
{
    const auto &xkbrules = XkbRules::instance();
    QString englishName;

    if (uniqueName.startsWith("keyboard-")) {
        auto layout = xkbrules.layout(uniqueName);
        englishName = layout.description;
    }

    if (!englishName.isEmpty()) {
        return englishName;
    }

    if (langCode.isEmpty()) {
        return "Unknown";
    } else if (langCode == "*") {
        return "Multilingual";
    }

    englishName = QLocale::languageToString(QLocale(langCode).language());
    if (englishName.isEmpty()) {
        englishName = "Unknown";
        qInfo("NOTICE: uniqueName [%s] not found name. unknown.", uniqueName.toStdString().c_str());
    }

    return englishName;
}

static QString buildKeyByIMNameAndLanguageCode(const QString &uniqueName,
                                               const QString &languageCode)
{
    auto translatedName = languageName(uniqueName, languageCode);
    auto englishName = getEnglishLanguageName(uniqueName, languageCode);
    QString key = translatedName + " - " + englishName;

    return key;
}

int IMWorker::m_enabledLanguageCount = 0;

IMWorker *IMWorker::instance()
{
    static IMWorker worker;
    return &worker;
}

IMWorker::IMWorker(QObject *parent)
    : QObject(parent)
    , m_interface(new DDBusInterface(server, path, interface, QDBusConnection::sessionBus()))
{
    fetchGroups();
    fetchAvailableInputMethodEntries();
}

void IMWorker::setInitializedStatus(const bool initialized)
{
    if (m_initialized == initialized) {
        return;
    }
    m_initialized = initialized;
}

void IMWorker::fetchConfig(const QString &uri)
{
    QDBusPendingCall call = m_interface->asyncCall("GetConfig", uri);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            this,
            [=](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<QDBusVariant, ConfigTypeList> reply = *watcher;
                if (reply.isError()) {
                    qWarning() << "GetConfig error:" << reply.reply().errorMessage();
                    return;
                }

                if (!m_initialized) {
                    auto desc = reply.argumentAt<1>();
                    if (!desc.size()) {
                        return;
                    }

                    QMap<QString, ConfigOptionList> descs;
                    for (auto &type : desc) {
                        descs[type.name()] = type.options();
                    }
                    /* m_mainType = desc[0].name(); */
                    // TODO:  replaced by a signal
                    /* setupWidget(mainWidget_, mainType_, QString()); */
                    Q_EMIT fetchConfigDescriptionFinished(descs, desc[0].name());
                    m_initialized = true;
                }

                if (m_initialized) {
                    // TODO:  replaced by a signal
                    /* setValue(reply.argumentAt<0>().variant()); */
                    Q_EMIT fetchConfigValueFinished(reply.argumentAt<0>().variant());
                }
                watcher->deleteLater();
            });
}

void IMWorker::fetchAvailableInputMethodEntries()
{
    QDBusPendingCall call = m_interface->asyncCall("AvailableInputMethods");
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            this,
            [=](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<InputMethodEntryList> reply = *watcher;
                if (reply.isError()) {
                    qWarning() << "AvailableInputMethods:" << reply.reply().errorMessage();
                    return;
                }
                m_allIMEntries = reply.value();
                updateEnabledIM(m_allIMEntries, m_imEntries);
                Q_EMIT fetchAvailableInputMethodEntriesFinished(m_allIMEntries);
            });
}

void IMWorker::fetchGroups()
{
    QDBusPendingCall call = m_interface->asyncCall("InputMethodGroups");
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            this,
            [=](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<QStringList> reply = *watcher;
                if (reply.isError()) {
                    qWarning() << "InputMethodGroups:" << reply.reply().errorMessage();
                    return;
                }
                m_groups = reply.value();
                Q_EMIT fetchGroupsFinished(m_groups);
                if (!m_groups.empty()) {
                    fetchCurrentGroupInfo(m_groups.front());
                }
            });
}

void IMWorker::fetchCurrentGroupInfo(const QString &group)
{
    if (m_currentGroup != group) {
        m_currentGroup = group;
        Q_EMIT currentGroupChanged(m_currentGroup);
    }
    QDBusPendingCall call = m_interface->asyncCall("InputMethodGroupInfo", group);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            this,
            [=](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<QString, KeyValueList> reply = *watcher;
                if (reply.isError()) {
                    m_defaultLayout.clear();
                    m_imEntries.clear();
                    qWarning() << "InputMethodGroupInfo:" << reply.reply().errorMessage();
                    return;
                } else {
                    m_defaultLayout = reply.argumentAt<0>();
                    m_imEntries = reply.argumentAt<1>();
                    updateEnabledIM(m_allIMEntries, m_imEntries);
                }

                /* Q_EMIT fetchGroupInfoFinished(reply.value()); */
            });
}

void IMWorker::updateEnabledIM(const InputMethodEntryList &imEntryList,
                               const KeyValueList &enabledIMList)
{
    m_enabledIMEntries.clear();
    m_availableIMEntryList.clear();
    m_enabledLanguages.clear();

    m_enabledLanguageCount = enabledIMList.size();
    for (InputMethodEntry imEntry : imEntryList) {
        auto it =
            std::find_if(enabledIMList.begin(), enabledIMList.end(), [imEntry](const KeyValue e) {
                return e.key() == imEntry.uniqueName();
            });
        if (it != enabledIMList.end()) {
            imEntry.setEnabled(true);
        }
        QString key = buildKeyByIMNameAndLanguageCode(imEntry.uniqueName(), imEntry.languageCode());
        InputMethodEntryList &values = m_availableIMEntryList[key];
        values.append(imEntry);
    }
    for (const auto &im : enabledIMList) {
        auto iter =
            std::find_if(imEntryList.begin(), imEntryList.end(), [im](const InputMethodEntry e) {
                return e.uniqueName() == im.key();
            });
        if (iter != imEntryList.end()) {
            m_enabledIMEntries.append(*iter);
        }
    }

    for (const InputMethodEntry &im : imEntryList) {
        auto iter = std::find_if(m_enabledIMEntries.begin(),
                                 m_enabledIMEntries.end(),
                                 [im](const InputMethodEntry e) {
                                     return e.uniqueName() == im.uniqueName();
                                 });
        if (iter != m_enabledIMEntries.end()) {
            m_enabledLanguages << iter->languageCode().left(2);
        }
    }
    Q_EMIT fetchAvailableIMEntriesFinished(m_availableIMEntryList);
    Q_EMIT enabledIMEntriesChanged(m_enabledIMEntries);
}

void IMWorker::launchStore()
{
    DDBusSender()
        .service("com.home.appstore.client")
        .interface("com.home.appstore.client")
        .path("/com/home/appstore/client")
        .method("newInstence")
        .call();
}

bool IMWorker::compare(const QString &left, const QString &right)
{
    QPair<bool, CONTAIN_CUR_LANG> leftValue = checkCategories(left);
    QPair<bool, CONTAIN_CUR_LANG> rightValue = checkCategories(right);
    bool leftContainsEnabledIM = leftValue.first;
    bool rightContainsEnabledIM = rightValue.first;
    CONTAIN_CUR_LANG leftContainsCurrentLanguage = leftValue.second;
    CONTAIN_CUR_LANG rightContainsCurrentLanguage = rightValue.second;

    if (leftContainsEnabledIM != rightContainsEnabledIM) {
        return leftContainsEnabledIM ? true : false;
    }

    if (leftContainsCurrentLanguage == rightContainsCurrentLanguage) {
        QString l = left;
        QString r = right;
        return QCollator().compare(l, r) < 0;
        return 0;
    }

    if (leftContainsCurrentLanguage == CONTAIN_CUR_LANG::YES) {
        return true;
    }

    if (rightContainsCurrentLanguage == CONTAIN_CUR_LANG::YES) {
        return false;
    }

    if (leftContainsCurrentLanguage == CONTAIN_CUR_LANG::DIFF_VARIANT) {
        return true;
    }

    if (rightContainsCurrentLanguage == CONTAIN_CUR_LANG::DIFF_VARIANT) {
        return false;
    }

    return 0;
}

QPair<bool, CONTAIN_CUR_LANG> IMWorker::checkCategories(const QString &key)
{
    QPair<bool, CONTAIN_CUR_LANG> ret{ false, CONTAIN_CUR_LANG::NO };
    bool containsEnable = false;
    CONTAIN_CUR_LANG containsCurrentLangugae = CONTAIN_CUR_LANG::NO;
    InputMethodEntryList imEntries;
    for (const auto &values : m_availableIMEntryList.values()) {
        for (const auto &value : values) {
            if (key == value.uniqueName()) {
                imEntries = values;
                break;
            }
        }
    }
    for (const InputMethodEntry &imEntry : imEntries) {
        if (imEntry.enabled()) {
            containsEnable = true;
        }
        QString lang = imEntry.languageCode();
        if (lang.isEmpty()) {
            continue;
        }
        if (QLocale().name() == lang) {
            containsCurrentLangugae = CONTAIN_CUR_LANG::YES;
        }

        if (containsCurrentLangugae == CONTAIN_CUR_LANG::NO) {
            if (QLocale().name().startsWith(lang.left(2))) {
                containsCurrentLangugae = CONTAIN_CUR_LANG::DIFF_VARIANT;
            }
        }
    }
    return ret;
}
