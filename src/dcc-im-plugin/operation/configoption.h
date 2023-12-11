// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CONFIGOPTION_H
#define CONFIGOPTION_H
#include <qdbusextratypes.h>
#include <qobject.h>
#include <qt6/QtCore/qtmetamacros.h>

#include <QDBusArgument>

namespace dcc_im_plugin {
namespace operation {

class ConfigOption
{
public:
    ConfigOption() = default;
    ConfigOption(const ConfigOption &option);
    ~ConfigOption();

    QString name() const;
    void setName(const QString &name);

    QString type() const;
    void setType(const QString &type);

    QString description() const;
    void setDescription(const QString &description);

    QDBusVariant defaultValue() const;
    void setDefaultValue(const QDBusVariant &dbusVaraint);

    QVariantMap properties() const;
    void setProperties(const QVariantMap &variantMap);

    bool operator==(const ConfigOption &configOption);

private:
    QString m_name;
    QString m_type;
    QString m_description;
    QDBusVariant m_defaultValue;
    QVariantMap m_properties;
};

using ConfigOptionList = QList<ConfigOption>;

} // namespace operation
} // namespace dcc_im_plugin

QDBusArgument &operator<<(QDBusArgument &argument,
                          const dcc_im_plugin::operation::ConfigOption &value);
const QDBusArgument &operator>>(const QDBusArgument &argument,
                                dcc_im_plugin::operation::ConfigOption &value);

Q_DECLARE_METATYPE(dcc_im_plugin::operation::ConfigOption)
Q_DECLARE_METATYPE(dcc_im_plugin::operation::ConfigOptionList)

#endif
