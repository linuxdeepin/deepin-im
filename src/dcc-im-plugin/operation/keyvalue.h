// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KEYVALUE_H
#define KEYVALUE_H

#include <QDBusArgument>
#include <QString>

namespace dcc_im_plugin {
namespace operation {

class KeyValue
{
public:
    QString key() const;
    void setKey(const QString &key);
    QString value() const;
    void setValue(const QString &value);

private:
    QString m_key;
    QString m_value;
};

using KeyValueList = QList<KeyValue>;
} // namespace operation
} // namespace dcc_im_plugin

QDBusArgument &operator<<(QDBusArgument &argument, const dcc_im_plugin::operation::KeyValue &value);
const QDBusArgument &operator>>(const QDBusArgument &argument,
                                dcc_im_plugin::operation::KeyValue &value);
Q_DECLARE_METATYPE(dcc_im_plugin::operation::KeyValue)
#endif // !KEYVALUE_H
