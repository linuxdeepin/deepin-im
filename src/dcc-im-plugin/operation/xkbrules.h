// SPDX-FileCopyrightText: 2023 Deepin Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef _CONFIGLIB_XKBRULES_H_
#define _CONFIGLIB_XKBRULES_H_

#include <QHash>
#include <QString>

namespace dcc_im_plugin {
namespace operation {

class XkbRules
{
public:
    struct layout
    {
        QString name;
        QString shortDescription;
        QString description;
    };

    static XkbRules &instance();

    bool open(const QString &filename);

    layout layout(const QString &keyboard) const { return m_keyboardLayoutsMap.value(keyboard); }

    static QString tr(const QString &text);

private:
    XkbRules() = default;

    QHash<QString, struct layout> m_keyboardLayoutsMap;
};
} // namespace operation
} // namespace dcc_im_plugin

#endif
