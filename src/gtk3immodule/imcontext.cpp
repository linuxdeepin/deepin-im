// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "imcontext.h"

#include "wayland-dim-text-input-unstable-v1-client-protocol.h"
#include "wl/client/Connection.h"
#include "wl/client/ConnectionRaw.h"
#include "wl/client/Seat.h"
#include "wl/client/ZwpDimTextInputManagerV1.h"
#include "wl/client/ZwpDimTextInputV1.h"

#include <gdk/gdkprivate.h>
#include <gdk/gdkwayland.h>

#define POINT_TRANSFORM(p) (DimIMContextWaylandGlobal *)(p)

struct _DimIMContextWaylandGlobal
{
    GtkIMContext *current;
    static const zwp_dim_text_input_v1_listener tiListener;
    std::shared_ptr<wl::client::ZwpDimTextInputV1> ti;
    wl::client::ConnectionBase *wl;

    guint serial;
    guint doneSerial;

    // wayland listeners
    static void textInputModifiersMap(void *data,
                                      struct zwp_dim_text_input_v1 *zwpDimTextInputV1,
                                      struct wl_array *map);
    static void textInputPreedit(void *data,
                                 zwp_dim_text_input_v1 *zwpDimTextInputV1,
                                 const char *text,
                                 int32_t cursorBegin,
                                 int32_t cursorEnd);
    static void textInputCommit(void *data,
                                zwp_dim_text_input_v1 *zwpDimTextInputV1,
                                const char *text);
    static void textInputDeleteSurroundingText(void *data,
                                               zwp_dim_text_input_v1 *zwpDimTextInputV1,
                                               uint32_t beforeLength,
                                               uint32_t afterLength);
    static void textInputDone(void *data,
                              zwp_dim_text_input_v1 *zwpDimTextInputV1,
                              uint32_t serial);
    static void textInputKeysym(void *data,
                                struct zwp_dim_text_input_v1 *zwpDimTextInputV1,
                                uint32_t serial,
                                uint32_t time,
                                uint32_t sym,
                                uint32_t state,
                                uint32_t modifiers);
};

const zwp_dim_text_input_v1_listener DimIMContextWaylandGlobal::tiListener = {
    textInputModifiersMap,          textInputPreedit, textInputCommit,
    textInputDeleteSurroundingText, textInputDone,    textInputKeysym
};

struct preedit
{
    char *text;
    int cursorBegin;
    int cursorEnd;
};

struct surroundingDelete
{
    guint beforeLength;
    guint afterLength;
};

struct _DimIMContext
{
    GtkIMContextSimple parent;

    GdkWindow *window;

    struct
    {
        char *text;
        int cursorIdx;
    } surrounding;

    enum zwp_text_input_v3_change_cause surroundingChange;

    struct surroundingDelete pendingSurroundingDelete;

    struct preedit currentPreedit;
    struct preedit pendingPreedit;

    char *pendingCommit;

    cairo_rectangle_int_t cursorRect;
    guint usePreedit : 1;
};

G_DEFINE_DYNAMIC_TYPE(DimIMContext, dim_im_context, GTK_TYPE_IM_CONTEXT);

/* functions prototype start */
#if GTK_CHECK_VERSION(3, 98, 4)
static void dim_im_context_set_client_window(GtkIMContext *context, GdkWidget *client);
#else
static void dimImContextSetClientWindow(GtkIMContext *context, GdkWindow *client);
#endif
static gboolean dimImContextFilterKeypress(GtkIMContext *context, GdkEventKey *key);
static void dimImContextReset(GtkIMContext *context);
static void dimImContextFocusIn(GtkIMContext *context);
static void dimImContextFocusOut(GtkIMContext *context);
static void dimImContextSetCursorLocation(GtkIMContext *context, GdkRectangle *area);
static void dimImContextSetUsePreedit(GtkIMContext *context, gboolean usePreedit);
static void
dimImContextSetSurrounding(GtkIMContext *context, const gchar *text, gint len, gint cursorIndex);
static void dimImContextGetPreeditString(GtkIMContext *context,
                                         gchar **str,
                                         PangoAttrList **attrs,
                                         gint *cursorPos);
static gboolean dimImContextGetSurrounding(GtkIMContext *context, gchar **text, gint *cursorIndex);
static DimIMContextWaylandGlobal *dimImContextWaylandGlobalGet(GdkDisplay *display);
static DimIMContextWaylandGlobal *dimImContextWaylandGetGlobal(DimIMContext *self);
static void textInputPreeditApply(DimIMContextWaylandGlobal *global);

/* functions prototype end */

// virtual functions start

static DimIMContextWaylandGlobal *dimImContextWaylandGetGlobal(DimIMContext *self)
{
    if (self->window == nullptr)
        return nullptr;

    DimIMContextWaylandGlobal *global =
        dimImContextWaylandGlobalGet(gdk_window_get_display(self->window));

    if (!global) {
        return nullptr;
    }

    if (global->current != GTK_IM_CONTEXT(self))
        return nullptr;

    return global;
}

static void notifySurroundingText(DimIMContext *context)
{
#define MAX_LEN 4000
    const char *start, *end;
    int len, cursor, anchor;
    char *str = nullptr;

    if (!context->surrounding.text)
        return;
    DimIMContextWaylandGlobal *global = dimImContextWaylandGetGlobal(context);
    if (global == nullptr)
        return;

    len = strlen(context->surrounding.text);
    cursor = context->surrounding.cursorIdx;

    /* The protocol specifies a maximum length of 4KiB on transfers,
     * mangle the surrounding text if it's bigger than that, and relocate
     * cursor/anchor locations as per the string being sent.
     */
    if (len > MAX_LEN) {
        if (context->surrounding.cursorIdx < MAX_LEN) {
            start = context->surrounding.text;
            end = &context->surrounding.text[MAX_LEN];
        } else if (context->surrounding.cursorIdx > len - MAX_LEN) {
            start = &context->surrounding.text[len - MAX_LEN];
            end = &context->surrounding.text[len];
        } else {
            int mid, a, b;
            int cursor_len = ABS(context->surrounding.cursorIdx);

            if (cursor_len > MAX_LEN) {
                g_warn_if_reached();
                return;
            }

            mid = context->surrounding.cursorIdx + (cursor_len / 2);
            a = MAX(0, mid - (MAX_LEN / 2));
            b = MIN(len, mid + (MAX_LEN / 2));

            start = &context->surrounding.text[a];
            end = &context->surrounding.text[b];
        }

        if (start != context->surrounding.text)
            start = g_utf8_next_char(start);
        if (end != &context->surrounding.text[len])
            end = g_utf8_find_prev_char(context->surrounding.text, end);

        cursor -= start - context->surrounding.text;
        anchor -= start - context->surrounding.text;

        str = g_strndup(start, end - start);
    }

    global->ti->setSurroundingText(str ? str : context->surrounding.text, cursor, anchor);
    global->ti->setTextChangeCause(context->surroundingChange);
    g_free(str);
#undef MAX_LEN
}

static uint32_t translateHints(GtkInputHints inputHints, GtkInputPurpose purpose)
{
    uint32_t hints = 0;

    if (inputHints & GTK_INPUT_HINT_SPELLCHECK)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_SPELLCHECK;
    if (inputHints & GTK_INPUT_HINT_WORD_COMPLETION)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_COMPLETION;
    if (inputHints & GTK_INPUT_HINT_LOWERCASE)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_LOWERCASE;
    if (inputHints & GTK_INPUT_HINT_UPPERCASE_CHARS)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_UPPERCASE;
    if (inputHints & GTK_INPUT_HINT_UPPERCASE_WORDS)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_TITLECASE;
    if (inputHints & GTK_INPUT_HINT_UPPERCASE_SENTENCES)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_AUTO_CAPITALIZATION;

    if (purpose == GTK_INPUT_PURPOSE_PIN || purpose == GTK_INPUT_PURPOSE_PASSWORD) {
        hints |= (ZWP_TEXT_INPUT_V3_CONTENT_HINT_HIDDEN_TEXT
                  | ZWP_TEXT_INPUT_V3_CONTENT_HINT_SENSITIVE_DATA);
    }

    return hints;
}

static uint32_t translatePurpose(GtkInputPurpose purpose)
{
    switch (purpose) {
    case GTK_INPUT_PURPOSE_FREE_FORM:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_NORMAL;
    case GTK_INPUT_PURPOSE_ALPHA:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_ALPHA;
    case GTK_INPUT_PURPOSE_DIGITS:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_DIGITS;
    case GTK_INPUT_PURPOSE_NUMBER:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_NUMBER;
    case GTK_INPUT_PURPOSE_PHONE:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_PHONE;
    case GTK_INPUT_PURPOSE_URL:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_URL;
    case GTK_INPUT_PURPOSE_EMAIL:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_EMAIL;
    case GTK_INPUT_PURPOSE_NAME:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_NAME;
    case GTK_INPUT_PURPOSE_PASSWORD:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_PASSWORD;
    case GTK_INPUT_PURPOSE_PIN:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_PIN;
    case GTK_INPUT_PURPOSE_TERMINAL:
        return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_TERMINAL;
    default:
        g_assert_not_reached();
    }

    return ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_NORMAL;
}

static void notifyContentType(DimIMContext *context)
{
    GtkInputHints hints;
    GtkInputPurpose purpose;

    DimIMContextWaylandGlobal *global = dimImContextWaylandGetGlobal(context);
    if (global == nullptr)
        return;

    g_object_get(context, "input-hints", &hints, "input-purpose", &purpose, nullptr);

    global->ti->setContentType(translateHints(hints, purpose), translatePurpose(purpose));
}

static void commitState(DimIMContext *context)
{
    DimIMContextWaylandGlobal *global = dimImContextWaylandGetGlobal(context);

    if (global == nullptr)
        return;

    global->serial++;
    global->ti->commit();
    context->surroundingChange = ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_INPUT_METHOD;
}

static void notifyCursorLocation(DimIMContext *context)
{
    DimIMContextWaylandGlobal *global = dimImContextWaylandGetGlobal(context);
    if (global == nullptr)
        return;

    if (context->window) {
        cairo_rectangle_int_t rect = context->cursorRect;
        gdk_window_get_root_coords(context->window, rect.x, rect.y, &rect.x, &rect.y);
        global->ti->setCursorRectangle(rect.x, rect.y, rect.width, rect.height);
    }
}

static void notifyImChange(DimIMContext *context, enum zwp_text_input_v3_change_cause cause)
{
    gboolean result;

    DimIMContextWaylandGlobal *global = dimImContextWaylandGetGlobal(context);
    if (global == nullptr)
        return;

    context->surroundingChange = cause;

    g_signal_emit_by_name(global->current, "retrieve-surrounding", &result);
    notifySurroundingText(context);
    notifyContentType(context);
    notifyCursorLocation(context);
    commitState(context);
}

void DimIMContextWaylandGlobal::textInputModifiersMap(
    void *data, struct zwp_dim_text_input_v1 *zwpDimTextInputV1, struct wl_array *map)
{
    // TODO
}

void DimIMContextWaylandGlobal::textInputKeysym(void *data,
                                                struct zwp_dim_text_input_v1 *zwpDimTextInputV1,
                                                uint32_t serial,
                                                uint32_t time,
                                                uint32_t sym,
                                                uint32_t state,
                                                uint32_t modifiers)
{
    // TODO
}

void DimIMContextWaylandGlobal::textInputPreedit(void *data,
                                                 zwp_dim_text_input_v1 *zwpDimTextInputV1,
                                                 const char *text,
                                                 int32_t cursorBegin,
                                                 int32_t cursorEnd)
{
    DimIMContextWaylandGlobal *global = POINT_TRANSFORM(data);

    if (!global->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global->current);

    g_free(context->pendingPreedit.text);
    context->pendingPreedit.text = g_strdup(text);
    context->pendingPreedit.cursorBegin = cursorBegin;
    context->pendingPreedit.cursorEnd = cursorEnd;
}

static void enable(DimIMContext *context, DimIMContextWaylandGlobal *global)
{
    global->ti->enable();
    notifyImChange(context, ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_OTHER);
}

static void disable(DimIMContext *context, DimIMContextWaylandGlobal *global)
{
    global->ti->disable();

    /* The commit above will still count in the .done event accounting,
     * we should account for it, lest the serial gets out of sync after
     * a future focus_in/enable.
     */
    global->doneSerial++;

    /* after disable, incoming state changes won't take effect anyway */
    if (context->currentPreedit.text) {
        global->textInputPreedit(global, global->ti->get(), nullptr, 0, 0);
        textInputPreeditApply(global);
    }
}

void DimIMContextWaylandGlobal::textInputCommit(void *data,
                                                zwp_dim_text_input_v1 *zwpDimTextInputV1,
                                                const char *text)
{
    DimIMContextWaylandGlobal *global = POINT_TRANSFORM(data);

    if (!global->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global->current);

    g_free(context->pendingCommit);
    context->pendingCommit = g_strdup(text);
}

void DimIMContextWaylandGlobal::textInputDeleteSurroundingText(
    void *data,
    zwp_dim_text_input_v1 *zwpDimTextInputV1,
    uint32_t beforeLength,
    uint32_t afterLength)
{
    DimIMContextWaylandGlobal *global = POINT_TRANSFORM(data);

    if (!global->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global->current);

    /* We already got byte lengths from text_input_v3, but GTK uses char lengths
     * for delete_surrounding, So convert it here.
     */
    char *cursor_pointer = context->surrounding.text + context->surrounding.cursorIdx;
    uint32_t char_before_length =
        g_utf8_pointer_to_offset(cursor_pointer - beforeLength, cursor_pointer);
    uint32_t char_after_length =
        g_utf8_pointer_to_offset(cursor_pointer, cursor_pointer + afterLength);

    context->pendingSurroundingDelete.beforeLength = char_before_length;
    context->pendingSurroundingDelete.afterLength = char_after_length;
}

static void textInputCommitApply(DimIMContextWaylandGlobal *global)
{
    DimIMContext *context = DIM_IM_CONTEXT(global->current);
    if (context->pendingCommit)
        g_signal_emit_by_name(global->current, "commit", context->pendingCommit);
    g_free(context->pendingCommit);
    context->pendingCommit = nullptr;
}

static void textInputDeleteSurroundingTextApply(DimIMContextWaylandGlobal *global)
{
    gboolean retval;
    struct surroundingDelete defaults = { 0 };

    DimIMContext *context = DIM_IM_CONTEXT(global->current);

    gint len = context->pendingSurroundingDelete.afterLength
        + context->pendingSurroundingDelete.beforeLength;
    if (len > 0) {
        g_signal_emit_by_name(global->current,
                              "delete-surrounding",
                              -context->pendingSurroundingDelete.beforeLength,
                              len,
                              &retval);
        notifyImChange(DIM_IM_CONTEXT(context), ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_INPUT_METHOD);
    }

    context->pendingSurroundingDelete = defaults;
}

static void textInputPreeditApply(DimIMContextWaylandGlobal *global)
{
    struct preedit defaults = { 0 };

    if (!global->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global->current);
    if (context->pendingPreedit.text == nullptr && context->currentPreedit.text == nullptr)
        return;

    gboolean state_change =
        ((context->pendingPreedit.text == nullptr) != (context->currentPreedit.text == nullptr));

    if (state_change && !context->currentPreedit.text)
        g_signal_emit_by_name(context, "preedit-start");

    g_free(context->currentPreedit.text);
    context->currentPreedit = context->pendingPreedit;
    context->pendingPreedit = defaults;

    g_signal_emit_by_name(context, "preedit-changed");

    if (state_change && !context->currentPreedit.text)
        g_signal_emit_by_name(context, "preedit-end");
}

void DimIMContextWaylandGlobal::textInputDone(void *data,
                                              zwp_dim_text_input_v1 *zwpDimTextInputV1,
                                              uint32_t serial)
{
    DimIMContextWaylandGlobal *global = POINT_TRANSFORM(data);

    global->doneSerial = serial;

    if (!global->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global->current);
    gboolean update_im =
        (context->pendingCommit != nullptr
         || g_strcmp0(context->pendingPreedit.text, context->currentPreedit.text) != 0);

    textInputDeleteSurroundingTextApply(global);
    textInputCommitApply(global);
    textInputPreeditApply(global);

    if (update_im && global->serial == serial)
        notifyImChange(context, ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_INPUT_METHOD);
}

static void gtkImContextWaylandGlobalFree(gpointer data)
{
    DimIMContextWaylandGlobal *global = POINT_TRANSFORM(data);

    g_free(global);
}

static DimIMContextWaylandGlobal *dimImContextWaylandGlobalGet(GdkDisplay *display)
{
    DimIMContextWaylandGlobal *global =
        (DimIMContextWaylandGlobal *)g_object_get_data(G_OBJECT(display),
                                                       "dim-im-context-wayland-global");
    if (global != nullptr)
        return global;

    global = g_new0(DimIMContextWaylandGlobal, 1);

    wl_display *gdk_wayland_display = gdk_wayland_display_get_wl_display(display);
    if (gdk_wayland_display != nullptr) {
        global->wl = new wl::client::ConnectionRaw(gdk_wayland_display);
    } else {
        auto wl = new wl::client::Connection("imfakewl");
        global->wl = wl;
        GIOChannel *channel = g_io_channel_unix_new(wl->getFd());
        g_io_add_watch(
            channel,
            (GIOCondition)(G_IO_IN),
            [](GIOChannel *source, GIOCondition condition, gpointer data) -> gboolean {
                auto *conn = static_cast<wl::client::Connection *>(data);
                return conn->dispatch();
            },
            wl);
    }

    auto seat = global->wl->getGlobal<wl::client::Seat>();
    auto tiManager = global->wl->getGlobal<wl::client::ZwpDimTextInputManagerV1>();

    global->ti = tiManager->getTextInput(seat);

    zwp_dim_text_input_v1_add_listener(global->ti->get(),
                                       &DimIMContextWaylandGlobal::tiListener,
                                       global);
    global->wl->flush();

    g_object_set_data_full(G_OBJECT(display),
                           "dim-im-context-wayland-global",
                           global,
                           gtkImContextWaylandGlobalFree);

    return global;
}

#if GTK_CHECK_VERSION(3, 98, 4)
static void dim_im_context_set_client_window(GtkIMContext *context, GdkWidget *client)
#else
static void dimImContextSetClientWindow(GtkIMContext *context, GdkWindow *client)
#endif
{
    g_return_if_fail(GTK_IS_IM_CONTEXT(context));
    g_return_if_fail(client);

    DimIMContext *contextWayland = DIM_IM_CONTEXT(context);

    if (client == contextWayland->window)
        return;

    if (contextWayland->window)
        dimImContextFocusOut(context);

    g_set_object(&contextWayland->window, client);
}

static gboolean dimImContextFilterKeypress(GtkIMContext *context, GdkEventKey *event)
{
    /* This is done by the compositor */
    return GTK_IM_CONTEXT_CLASS(dim_im_context_parent_class)->filter_keypress(context, event);
}

static void dimImContextFocusIn(GtkIMContext *context)
{
    g_return_if_fail(GTK_IS_IM_CONTEXT(context));

    DimIMContext *self = DIM_IM_CONTEXT(context);

    if (self->window == nullptr)
        return;

    DimIMContextWaylandGlobal *global =
        dimImContextWaylandGlobalGet(gdk_window_get_display(self->window));
    if (!global) {
        return;
    }

    if (global->current == context)
        return;

    global->current = context;
    if (!global->ti)
        return;
    enable(self, global);
}

static void dimImContextFocusOut(GtkIMContext *context)
{
    DimIMContext *self = DIM_IM_CONTEXT(context);

    DimIMContextWaylandGlobal *global = dimImContextWaylandGetGlobal(self);
    if (global == nullptr)
        return;

    disable(self, global);

    global->current = nullptr;
}

static void dimImContextReset(GtkIMContext *context)
{
    DimIMContext *self = DIM_IM_CONTEXT(context);
    DimIMContextWaylandGlobal *global = dimImContextWaylandGetGlobal(self);
    if (global == nullptr)
        return;
    notifyImChange(DIM_IM_CONTEXT(context), ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_OTHER);
}

static void dimImContextWaylandCommit(GtkIMContext *context, const gchar *str)
{
    if (GTK_IM_CONTEXT_CLASS(dim_im_context_parent_class)->commit)
        GTK_IM_CONTEXT_CLASS(dim_im_context_parent_class)->commit(context, str);

    notifyImChange(DIM_IM_CONTEXT(context), ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_INPUT_METHOD);
}

static void dimImContextSetCursorLocation(GtkIMContext *context, GdkRectangle *area)
{
    DimIMContext *contextWayland = DIM_IM_CONTEXT(context);

    if (contextWayland->cursorRect.x == area->x && contextWayland->cursorRect.y == area->y
        && contextWayland->cursorRect.width == area->width
        && contextWayland->cursorRect.height == area->height)
        return;

    int side = contextWayland->cursorRect.height;
    contextWayland->cursorRect = *area;
}

static void dimImContextSetUsePreedit(GtkIMContext *context, gboolean usePreedit)
{
    DimIMContext *contextWayland = DIM_IM_CONTEXT(context);

    contextWayland->usePreedit = !!usePreedit;
}

static void
dimImContextSetSurrounding(GtkIMContext *context, const gchar *text, gint len, gint cursorIndex)
{
    DimIMContext *contextWayland = DIM_IM_CONTEXT(context);

    if (contextWayland->surrounding.text && text
        && (len < 0 || len == strlen(contextWayland->surrounding.text))
        && strncmp(contextWayland->surrounding.text, text, len) == 0
        && contextWayland->surrounding.cursorIdx == cursorIndex)
        return;

    g_free(contextWayland->surrounding.text);
    contextWayland->surrounding.text = g_strndup(text, len);
    contextWayland->surrounding.cursorIdx = cursorIndex;
}

static void dimImContextGetPreeditString(GtkIMContext *context,
                                         gchar **str,
                                         PangoAttrList **attrs,
                                         gint *cursor_pos)
{
    DimIMContext *contextWayland = DIM_IM_CONTEXT(context);

    if (attrs)
        *attrs = nullptr;

    GTK_IM_CONTEXT_CLASS(dim_im_context_parent_class)
        ->get_preedit_string(context, str, attrs, cursor_pos);

    /* If the parent implementation returns a len>0 string, go with it */
    if (str && *str) {
        if (**str)
            return;

        g_free(*str);
    }

    const char *preedit_str =
        contextWayland->currentPreedit.text ? contextWayland->currentPreedit.text : "";

    if (cursor_pos)
        *cursor_pos = g_utf8_strlen(preedit_str, contextWayland->currentPreedit.cursorBegin);

    if (str)
        *str = g_strdup(preedit_str);
    if (attrs) {
        PangoAttribute *attr;
        guint len = strlen(preedit_str);

        if (!*attrs)
            *attrs = pango_attr_list_new();

        attr = pango_attr_underline_new(PANGO_UNDERLINE_SINGLE);
        attr->start_index = 0;
        attr->end_index = len;
        pango_attr_list_insert(*attrs, attr);

        /* enable fallback, since IBus will send us things like ⎄ */
        attr = pango_attr_fallback_new(TRUE);
        attr->start_index = 0;
        attr->end_index = len;
        pango_attr_list_insert(*attrs, attr);

        if (contextWayland->currentPreedit.cursorBegin
            != contextWayland->currentPreedit.cursorEnd) {
            /* FIXME: Oh noes, how to highlight while taking into account user preferences? */
            PangoAttribute *cursor = pango_attr_weight_new(PANGO_WEIGHT_BOLD);
            cursor->start_index = contextWayland->currentPreedit.cursorBegin;
            cursor->end_index = contextWayland->currentPreedit.cursorEnd;
            pango_attr_list_insert(*attrs, cursor);
        }
    }
}

static gboolean dimImContextGetSurrounding(GtkIMContext *context, gchar **text, gint *cursorIndex)
{
    DimIMContext *contextWayland = DIM_IM_CONTEXT(context);

    if (!contextWayland->surrounding.text)
        return FALSE;

    *text = contextWayland->surrounding.text;
    *cursorIndex = contextWayland->surrounding.cursorIdx;
    return TRUE;
}

static void dim_im_context_finalize(GObject *obj)
{
    DimIMContext *context = DIM_IM_CONTEXT(obj);

    dimImContextFocusOut(GTK_IM_CONTEXT(context));

    g_clear_object(&context->window);
    g_free(context->surrounding.text);
    g_free(context->currentPreedit.text);
    g_free(context->pendingPreedit.text);
    g_free(context->pendingCommit);

    G_OBJECT_CLASS(dim_im_context_parent_class)->finalize(obj);
}

// virtual functions end

static void dim_im_context_class_init(DimIMContextClass *klass)
{
    GtkIMContextClass *im_context_class = GTK_IM_CONTEXT_CLASS(klass);
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

#if GTK_CHECK_VERSION(3, 98, 4)
    im_context_class->set_client_widget = dim_im_context_set_client_window;
#else
    im_context_class->set_client_window = dimImContextSetClientWindow;
#endif
    im_context_class->get_preedit_string = dimImContextGetPreeditString;
    im_context_class->filter_keypress = dimImContextFilterKeypress;
    im_context_class->focus_in = dimImContextFocusIn;
    im_context_class->focus_out = dimImContextFocusOut;
    im_context_class->reset = dimImContextReset;
    im_context_class->commit = dimImContextWaylandCommit;
    im_context_class->set_cursor_location = dimImContextSetCursorLocation;
    im_context_class->set_use_preedit = dimImContextSetUsePreedit;
    im_context_class->set_surrounding = dimImContextSetSurrounding;
    im_context_class->get_surrounding = dimImContextGetSurrounding;

    gobject_class->finalize = dim_im_context_finalize;
}

static void dim_im_context_class_finalize(DimIMContextClass *klass) { }

static void onContentTypeChanged(DimIMContext *context)
{
    notifyContentType(context);
    commitState(context);
}

static void dim_im_context_init(DimIMContext *context)
{
    context->usePreedit = TRUE;

    g_signal_connect_swapped(context,
                             "notify::input-purpose",
                             G_CALLBACK(onContentTypeChanged),
                             context);
    g_signal_connect_swapped(context,
                             "notify::input-hints",
                             G_CALLBACK(onContentTypeChanged),
                             context);
}

DimIMContext *dimImContextNew(void)
{
    GObject *obj = (GObject *)g_object_new(DIM_TYPE_IM_CONTEXT, nullptr);
    return DIM_IM_CONTEXT(obj);
}

void dim_im_context_register(GTypeModule *type_module)
{
    dim_im_context_register_type(type_module);
}
