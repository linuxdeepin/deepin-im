// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DimGtkTextInputV1.h"

#include "imcontext.h"

DimGtkTextInputV1::DimGtkTextInputV1(struct ::zwp_dim_text_input_v1 *text_input,
                                     DimIMContextWaylandGlobal *global)
    : wl::client::ZwpDimTextInputV1(text_input)
    , global_(global)
{
}

void DimGtkTextInputV1::zwp_dim_text_input_v1_enter() { }

void DimGtkTextInputV1::zwp_dim_text_input_v1_leave() { }

void DimGtkTextInputV1::zwp_dim_text_input_v1_modifiers_map(struct wl_array *map) { }

void DimGtkTextInputV1::zwp_dim_text_input_v1_preedit_string(const char *text,
                                                             int32_t cursor_begin,
                                                             int32_t cursor_end)
{
    if (!global_->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global_->current);

    g_free(context->pendingPreedit.text);
    context->pendingPreedit.text = g_strdup(text);
    context->pendingPreedit.cursorBegin = cursor_begin;
    context->pendingPreedit.cursorEnd = cursor_end;
}

void DimGtkTextInputV1::zwp_dim_text_input_v1_commit_string(const char *text)
{
    if (!global_->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global_->current);

    g_free(context->pendingCommit);
    context->pendingCommit = g_strdup(text);
}

void DimGtkTextInputV1::zwp_dim_text_input_v1_delete_surrounding_text(uint32_t before_length,
                                                                      uint32_t after_length)
{
    if (!global_->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global_->current);

    /* We already got byte lengths from text_input_v3, but GTK uses char lengths
     * for delete_surrounding, So convert it here.
     */
    char *cursor_pointer = context->surrounding.text + context->surrounding.cursorIdx;
    uint32_t char_before_length =
        g_utf8_pointer_to_offset(cursor_pointer - before_length, cursor_pointer);
    uint32_t char_after_length =
        g_utf8_pointer_to_offset(cursor_pointer, cursor_pointer + after_length);

    context->pendingSurroundingDelete.beforeLength = char_before_length;
    context->pendingSurroundingDelete.afterLength = char_after_length;
}

void DimGtkTextInputV1::zwp_dim_text_input_v1_done(uint32_t serial)
{
    global_->doneSerial = serial;

    if (!global_->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global_->current);

    gboolean update_im =
        (context->pendingCommit != nullptr
         || g_strcmp0(context->pendingPreedit.text, context->currentPreedit.text) != 0);

    textInputDeleteSurroundingTextApply(global_);
    textInputCommitApply(global_);
    textInputPreeditApply(global_);

    if (update_im && global_->serial == serial)
        notifyImChange(context, ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_INPUT_METHOD);
}

void DimGtkTextInputV1::zwp_dim_text_input_v1_keysym(
    uint32_t serial, uint32_t time, uint32_t sym, uint32_t state, uint32_t modifiers)
{
    if (!global_->current)
        return;
    GdkEvent *gdk_event = gdk_event_new(state ? GDK_KEY_PRESS : GDK_KEY_RELEASE);
    GdkEventKey *gdk_event_key = reinterpret_cast<GdkEventKey *>(gdk_event);

    DimIMContext *contextWayland = DIM_IM_CONTEXT(global_->current);

    gdk_event_key->type = state ? GDK_KEY_PRESS : GDK_KEY_RELEASE;
    gdk_event_key->window = g_object_ref(contextWayland->window);
    gdk_event_key->send_event = TRUE;
    gdk_event_key->time = time;
    gdk_event_key->keyval = sym;
    gdk_event_key->length = 1;
    gdk_event_key->string = nullptr;
    gdk_event_key->state = modifiers;

    gdk_event_key->send_event = TRUE;
    gdk_event_key->time = time;

    auto handled = gtk_im_context_filter_keypress(contextWayland->slave, &gdk_event->key);
    gdk_event_free(gdk_event);
}

DimGtkTextInputV1::~DimGtkTextInputV1() { }
