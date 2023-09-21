// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDTEXTINPUTV4_P_H
#define QWAYLANDTEXTINPUTV4_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QWaylandInputMethodEventBuilder.h"
#include "QWaylandTextInputInterface.h"
#include "wl/client/ZwpDimTextInputV1.h"

#include <QLoggingCategory>

struct wl_callback;
struct wl_callback_listener;

Q_DECLARE_LOGGING_CATEGORY(qLcQpaWaylandTextInput)

class QWaylandDisplay;

class DimTextInputV1 : public wl::client::ZwpDimTextInputV1,
                       public QtWaylandClient::QWaylandTextInputInterface
{
public:
    DimTextInputV1(struct ::zwp_dim_text_input_v1 *text_input);
    ~DimTextInputV1() override;

    void reset() override;
    void commit() override;
    void updateState(Qt::InputMethodQueries queries, uint32_t flags) override;

    QRectF keyboardRect() const override;

    void enable() override;
    void disable() override;

protected:
    void zwp_dim_text_input_v1_enter() override;
    void zwp_dim_text_input_v1_leave() override;
    void zwp_dim_text_input_v1_modifiers_map(struct wl_array *map) override;
    void zwp_dim_text_input_v1_preedit_string(const char *text,
                                              int32_t cursor_begin,
                                              int32_t cursor_end) override;
    void zwp_dim_text_input_v1_commit_string(const char *text) override;
    void zwp_dim_text_input_v1_delete_surrounding_text(uint32_t before_length,
                                                       uint32_t after_length) override;
    void zwp_dim_text_input_v1_done(uint32_t serial) override;
    void zwp_dim_text_input_v1_keysym(
        uint32_t serial, uint32_t time, uint32_t sym, uint32_t state, uint32_t modifiers) override;

private:
    QWaylandInputMethodEventBuilder m_builder;

    QList<Qt::KeyboardModifier> m_modifiersMap;

    struct PreeditInfo
    {
        QString text;
        int cursorBegin = 0;
        int cursorEnd = 0;

        void clear()
        {
            text.clear();
            cursorBegin = 0;
            cursorEnd = 0;
        }
    };

    PreeditInfo m_pendingPreeditString;
    PreeditInfo m_currentPreeditString;
    QString m_pendingCommitString;
    uint m_pendingDeleteBeforeText = 0;
    uint m_pendingDeleteAfterText = 0;

    QString m_surroundingText;
    int m_cursor;    // cursor position in QString
    int m_cursorPos; // cursor position in wayland index
    int m_anchorPos; // anchor position in wayland index
    uint32_t m_contentHint = 0;
    uint32_t m_contentPurpose = 0;
    QRect m_cursorRect;

    uint m_currentSerial = 0;

    bool m_condReselection = false;

    Qt::KeyboardModifiers modifiersToQtModifiers(uint32_t modifiers);
};

#endif // QWAYLANDTEXTINPUTV4_P_H
