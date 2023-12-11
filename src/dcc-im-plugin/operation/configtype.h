// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef CONFIGTYPE_H
#define CONFIGTYPE_H

#include "configoption.h"

#include <QString>

namespace dcc_im_plugin {
namespace operation {

class ConfigType
{
public:
    QString name() const;
    void setName(const QString &name);

    ConfigOptionList options() const;
    void setOptions(const ConfigOptionList &options);

private:
    QString m_name;
    ConfigOptionList m_options;
};

using ConfigTypeList = QList<ConfigType>;
} // namespace operation
}; // namespace dcc_im_plugin

QDBusArgument &operator<<(QDBusArgument &argument,
                          const dcc_im_plugin::operation::ConfigType &value);
const QDBusArgument &operator>>(const QDBusArgument &argument,
                                dcc_im_plugin::operation::ConfigType &value);
Q_DECLARE_METATYPE(dcc_im_plugin::operation::ConfigType)
#endif
