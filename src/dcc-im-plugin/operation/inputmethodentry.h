// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef INPUTMETHODENTRY_H
#define INPUTMETHODENTRY_H

#include <qlist.h>

#include <QDBusArgument>
#include <QString>

namespace dcc_im_plugin {
namespace operation {

class InputMethodEntry
{
public:
    QString uniqueName() const;
    void setUniqueName(const QString &uniqueName);
    QString name() const;
    void setName(const QString &name);
    QString nativeName() const;
    void setNativeName(const QString &nativeName);
    QString icon() const;
    void setIcon(const QString &icon);
    QString label() const;
    void setLabel(const QString &label);
    QString languageCode() const;
    void setLanguageCode(const QString &languageCode);
    bool configurable() const;
    void setConfigurable(const bool &configurable);
    bool enabled() const;
    void setEnabled(const bool enabled);

private:
    QString m_uniqueName;
    QString m_name;
    QString m_nativeName;
    QString m_icon;
    QString m_label;
    QString m_languageCode;
    bool m_configurable = false;
    bool m_enabled = false;
};

using InputMethodEntryList = QList<InputMethodEntry>;
} // namespace operation
} // namespace dcc_im_plugin

QDBusArgument &operator<<(QDBusArgument &argument,
                          const dcc_im_plugin::operation::InputMethodEntry &value);
const QDBusArgument &operator>>(const QDBusArgument &argument,
                                dcc_im_plugin::operation::InputMethodEntry &value);
Q_DECLARE_METATYPE(dcc_im_plugin::operation::InputMethodEntry)
#endif // !INPUTMETHODENTRY_H
