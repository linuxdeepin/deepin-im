// Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDINPUTMETHODEVENTBUILDER_H
#define QWAYLANDINPUTMETHODEVENTBUILDER_H

#include <QInputMethodEvent>
#include <private/qglobal_p.h>

QT_BEGIN_NAMESPACE

class QWaylandInputMethodEventBuilder
{
public:
    QWaylandInputMethodEventBuilder() = default;
    ~QWaylandInputMethodEventBuilder();

    void reset();

    void setCursorPosition(int32_t index, int32_t anchor);
    void setDeleteSurroundingText(uint32_t beforeLength, uint32_t afterLength);

    void setPreeditCursor(int32_t index);

    QInputMethodEvent *buildCommit(const QString &text);
    QInputMethodEvent *buildPreedit(const QString &text);

    static int indexFromWayland(const QString &text, int length, int base = 0);
    static int indexToWayland(const QString &text, int length, int base = 0);

    static int trimmedIndexFromWayland(const QString &text, int length, int base = 0);
private:
    QPair<int, int> replacementForDeleteSurrounding();

    int32_t m_anchor = 0;
    int32_t m_cursor = 0;
    uint32_t m_deleteBefore = 0;
    uint32_t m_deleteAfter = 0;

    int32_t m_preeditCursor = 0;
    QList<QInputMethodEvent::Attribute> m_preeditStyles;
};

struct QWaylandInputMethodContentType {
    uint32_t hint = 0;
    uint32_t purpose = 0;

    static QWaylandInputMethodContentType convertV4(Qt::InputMethodHints hints);
};


QT_END_NAMESPACE

#endif // QWAYLANDINPUTMETHODEVENTBUILDER_H
