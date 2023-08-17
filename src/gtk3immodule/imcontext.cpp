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
    guint done_serial;

    // wayland listeners
    static void text_input_modifiers_map(void *data,
                                         struct zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                         struct wl_array *map);
    static void text_input_preedit(void *data,
                                   zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                   const char *text,
                                   int32_t cursor_begin,
                                   int32_t cursor_end);
    static void text_input_commit(void *data,
                                  zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                  const char *text);
    static void text_input_delete_surrounding_text(void *data,
                                                   zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                                   uint32_t before_length,
                                                   uint32_t after_length);
    static void text_input_done(void *data,
                                zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                uint32_t serial);
    static void text_input_keysym(void *data,
                                  struct zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                  uint32_t serial,
                                  uint32_t time,
                                  uint32_t sym,
                                  uint32_t state,
                                  uint32_t modifiers);
};

const zwp_dim_text_input_v1_listener DimIMContextWaylandGlobal::tiListener = {
    text_input_modifiers_map,           text_input_preedit, text_input_commit,
    text_input_delete_surrounding_text, text_input_done,    text_input_keysym
};

struct preedit
{
    char *text;
    int cursor_begin;
    int cursor_end;
};

struct surrounding_delete
{
    guint before_length;
    guint after_length;
};

struct _DimIMContext
{
    GtkIMContextSimple parent;

    GdkWindow *window;

    struct
    {
        char *text;
        int cursor_idx;
    } surrounding;

    enum zwp_text_input_v3_change_cause surrounding_change;

    struct surrounding_delete pending_surrounding_delete;

    struct preedit current_preedit;
    struct preedit pending_preedit;

    char *pending_commit;

    cairo_rectangle_int_t cursor_rect;
    guint use_preedit : 1;
};

G_DEFINE_DYNAMIC_TYPE(DimIMContext, dim_im_context, GTK_TYPE_IM_CONTEXT);

/* functions prototype start */
#if GTK_CHECK_VERSION(3, 98, 4)
static void dim_im_context_set_client_window(GtkIMContext *context, GdkWidget *client);
#else
static void dim_im_context_set_client_window(GtkIMContext *context, GdkWindow *client);
#endif
static gboolean dim_im_context_filter_keypress(GtkIMContext *context, GdkEventKey *key);
static void dim_im_context_reset(GtkIMContext *context);
static void dim_im_context_focus_in(GtkIMContext *context);
static void dim_im_context_focus_out(GtkIMContext *context);
static void dim_im_context_set_cursor_location(GtkIMContext *context, GdkRectangle *area);
static void dim_im_context_set_use_preedit(GtkIMContext *context, gboolean use_preedit);
static void dim_im_context_set_surrounding(GtkIMContext *context,
                                           const gchar *text,
                                           gint len,
                                           gint cursor_index);
static void dim_im_context_get_preedit_string(GtkIMContext *context,
                                              gchar **str,
                                              PangoAttrList **attrs,
                                              gint *cursor_pos);
static gboolean dim_im_context_get_surrounding(GtkIMContext *context,
                                               gchar **text,
                                               gint *cursor_index);
static DimIMContextWaylandGlobal *dim_im_context_wayland_global_get(GdkDisplay *display);
static DimIMContextWaylandGlobal *dim_im_context_wayland_get_global(DimIMContext *self);
static void text_input_preedit_apply(DimIMContextWaylandGlobal *global);

/* functions prototype end */

// virtual functions start

static DimIMContextWaylandGlobal *dim_im_context_wayland_get_global(DimIMContext *self)
{
    if (self->window == nullptr)
        return nullptr;

    DimIMContextWaylandGlobal *global =
        dim_im_context_wayland_global_get(gdk_window_get_display(self->window));

    if (!global) {
        return nullptr;
    }

    if (global->current != GTK_IM_CONTEXT(self))
        return nullptr;

    return global;
}

static void notify_surrounding_text(DimIMContext *context)
{
#define MAX_LEN 4000
    const char *start, *end;
    int len, cursor, anchor;
    char *str = nullptr;

    if (!context->surrounding.text)
        return;
    DimIMContextWaylandGlobal *global = dim_im_context_wayland_get_global(context);
    if (global == nullptr)
        return;

    len = strlen(context->surrounding.text);
    cursor = context->surrounding.cursor_idx;

    /* The protocol specifies a maximum length of 4KiB on transfers,
     * mangle the surrounding text if it's bigger than that, and relocate
     * cursor/anchor locations as per the string being sent.
     */
    if (len > MAX_LEN) {
        if (context->surrounding.cursor_idx < MAX_LEN) {
            start = context->surrounding.text;
            end = &context->surrounding.text[MAX_LEN];
        } else if (context->surrounding.cursor_idx > len - MAX_LEN) {
            start = &context->surrounding.text[len - MAX_LEN];
            end = &context->surrounding.text[len];
        } else {
            int mid, a, b;
            int cursor_len = ABS(context->surrounding.cursor_idx);

            if (cursor_len > MAX_LEN) {
                g_warn_if_reached();
                return;
            }

            mid = context->surrounding.cursor_idx + (cursor_len / 2);
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
    global->ti->setTextChangeCause(context->surrounding_change);
    g_free(str);
#undef MAX_LEN
}

static uint32_t translate_hints(GtkInputHints input_hints, GtkInputPurpose purpose)
{
    uint32_t hints = 0;

    if (input_hints & GTK_INPUT_HINT_SPELLCHECK)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_SPELLCHECK;
    if (input_hints & GTK_INPUT_HINT_WORD_COMPLETION)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_COMPLETION;
    if (input_hints & GTK_INPUT_HINT_LOWERCASE)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_LOWERCASE;
    if (input_hints & GTK_INPUT_HINT_UPPERCASE_CHARS)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_UPPERCASE;
    if (input_hints & GTK_INPUT_HINT_UPPERCASE_WORDS)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_TITLECASE;
    if (input_hints & GTK_INPUT_HINT_UPPERCASE_SENTENCES)
        hints |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_AUTO_CAPITALIZATION;

    if (purpose == GTK_INPUT_PURPOSE_PIN || purpose == GTK_INPUT_PURPOSE_PASSWORD) {
        hints |= (ZWP_TEXT_INPUT_V3_CONTENT_HINT_HIDDEN_TEXT
                  | ZWP_TEXT_INPUT_V3_CONTENT_HINT_SENSITIVE_DATA);
    }

    return hints;
}

static uint32_t translate_purpose(GtkInputPurpose purpose)
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

static void notify_content_type(DimIMContext *context)
{
    GtkInputHints hints;
    GtkInputPurpose purpose;

    DimIMContextWaylandGlobal *global = dim_im_context_wayland_get_global(context);
    if (global == nullptr)
        return;

    g_object_get(context, "input-hints", &hints, "input-purpose", &purpose, nullptr);

    global->ti->setContentType(translate_hints(hints, purpose), translate_purpose(purpose));
}

static void commit_state(DimIMContext *context)
{
    DimIMContextWaylandGlobal *global = dim_im_context_wayland_get_global(context);

    if (global == nullptr)
        return;

    global->serial++;
    global->ti->commit();
    context->surrounding_change = ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_INPUT_METHOD;
}

static void notify_cursor_location(DimIMContext *context)
{
    DimIMContextWaylandGlobal *global = dim_im_context_wayland_get_global(context);
    if (global == nullptr)
        return;

    if (context->window) {
        cairo_rectangle_int_t rect = context->cursor_rect;
        gdk_window_get_root_coords(context->window, rect.x, rect.y, &rect.x, &rect.y);
        global->ti->setCursorRectangle(rect.x, rect.y, rect.width, rect.height);
    }
}

static void notify_im_change(DimIMContext *context, enum zwp_text_input_v3_change_cause cause)
{
    gboolean result;

    DimIMContextWaylandGlobal *global = dim_im_context_wayland_get_global(context);
    if (global == nullptr)
        return;

    context->surrounding_change = cause;

    g_signal_emit_by_name(global->current, "retrieve-surrounding", &result);
    notify_surrounding_text(context);
    notify_content_type(context);
    notify_cursor_location(context);
    commit_state(context);
}

void DimIMContextWaylandGlobal::text_input_modifiers_map(
    void *data, struct zwp_dim_text_input_v1 *zwp_dim_text_input_v1, struct wl_array *map)
{
    // TODO
}

void DimIMContextWaylandGlobal::text_input_keysym(
    void *data,
    struct zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
    uint32_t serial,
    uint32_t time,
    uint32_t sym,
    uint32_t state,
    uint32_t modifiers)
{
    // TODO
}

void DimIMContextWaylandGlobal::text_input_preedit(void *data,
                                                   zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                                   const char *text,
                                                   int32_t cursor_begin,
                                                   int32_t cursor_end)
{
    DimIMContextWaylandGlobal *global = POINT_TRANSFORM(data);

    if (!global->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global->current);

    g_free(context->pending_preedit.text);
    context->pending_preedit.text = g_strdup(text);
    context->pending_preedit.cursor_begin = cursor_begin;
    context->pending_preedit.cursor_end = cursor_end;
}

static void enable(DimIMContext *context, DimIMContextWaylandGlobal *global)
{
    global->ti->enable();
    notify_im_change(context, ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_OTHER);
}

static void disable(DimIMContext *context, DimIMContextWaylandGlobal *global)
{
    global->ti->disable();

    /* The commit above will still count in the .done event accounting,
     * we should account for it, lest the serial gets out of sync after
     * a future focus_in/enable.
     */
    global->done_serial++;

    /* after disable, incoming state changes won't take effect anyway */
    if (context->current_preedit.text) {
        global->text_input_preedit(global, global->ti->get(), nullptr, 0, 0);
        text_input_preedit_apply(global);
    }
}

void DimIMContextWaylandGlobal::text_input_commit(void *data,
                                                  zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                                  const char *text)
{
    DimIMContextWaylandGlobal *global = POINT_TRANSFORM(data);

    if (!global->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global->current);

    g_free(context->pending_commit);
    context->pending_commit = g_strdup(text);
}

void DimIMContextWaylandGlobal::text_input_delete_surrounding_text(
    void *data,
    zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
    uint32_t before_length,
    uint32_t after_length)
{
    DimIMContextWaylandGlobal *global = POINT_TRANSFORM(data);

    if (!global->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global->current);

    /* We already got byte lengths from text_input_v3, but GTK uses char lengths
     * for delete_surrounding, So convert it here.
     */
    char *cursor_pointer = context->surrounding.text + context->surrounding.cursor_idx;
    uint32_t char_before_length =
        g_utf8_pointer_to_offset(cursor_pointer - before_length, cursor_pointer);
    uint32_t char_after_length =
        g_utf8_pointer_to_offset(cursor_pointer, cursor_pointer + after_length);

    context->pending_surrounding_delete.before_length = char_before_length;
    context->pending_surrounding_delete.after_length = char_after_length;
}

static void text_input_commit_apply(DimIMContextWaylandGlobal *global)
{
    DimIMContext *context = DIM_IM_CONTEXT(global->current);
    if (context->pending_commit)
        g_signal_emit_by_name(global->current, "commit", context->pending_commit);
    g_free(context->pending_commit);
    context->pending_commit = nullptr;
}

static void text_input_delete_surrounding_text_apply(DimIMContextWaylandGlobal *global)
{
    gboolean retval;
    struct surrounding_delete defaults = { 0 };

    DimIMContext *context = DIM_IM_CONTEXT(global->current);

    gint len = context->pending_surrounding_delete.after_length
        + context->pending_surrounding_delete.before_length;
    if (len > 0) {
        g_signal_emit_by_name(global->current,
                              "delete-surrounding",
                              -context->pending_surrounding_delete.before_length,
                              len,
                              &retval);
        notify_im_change(DIM_IM_CONTEXT(context), ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_INPUT_METHOD);
    }

    context->pending_surrounding_delete = defaults;
}

static void text_input_preedit_apply(DimIMContextWaylandGlobal *global)
{
    struct preedit defaults = { 0 };

    if (!global->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global->current);
    if (context->pending_preedit.text == nullptr && context->current_preedit.text == nullptr)
        return;

    gboolean state_change =
        ((context->pending_preedit.text == nullptr) != (context->current_preedit.text == nullptr));

    if (state_change && !context->current_preedit.text)
        g_signal_emit_by_name(context, "preedit-start");

    g_free(context->current_preedit.text);
    context->current_preedit = context->pending_preedit;
    context->pending_preedit = defaults;

    g_signal_emit_by_name(context, "preedit-changed");

    if (state_change && !context->current_preedit.text)
        g_signal_emit_by_name(context, "preedit-end");
}

void DimIMContextWaylandGlobal::text_input_done(void *data,
                                                zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                                uint32_t serial)
{
    DimIMContextWaylandGlobal *global = POINT_TRANSFORM(data);

    global->done_serial = serial;

    if (!global->current)
        return;

    DimIMContext *context = DIM_IM_CONTEXT(global->current);
    gboolean update_im =
        (context->pending_commit != nullptr
         || g_strcmp0(context->pending_preedit.text, context->current_preedit.text) != 0);

    text_input_delete_surrounding_text_apply(global);
    text_input_commit_apply(global);
    text_input_preedit_apply(global);

    if (update_im && global->serial == serial)
        notify_im_change(context, ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_INPUT_METHOD);
}

static void gtk_im_context_wayland_global_free(gpointer data)
{
    DimIMContextWaylandGlobal *global = POINT_TRANSFORM(data);

    g_free(global);
}

static DimIMContextWaylandGlobal *dim_im_context_wayland_global_get(GdkDisplay *display)
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
                           gtk_im_context_wayland_global_free);

    return global;
}

#if GTK_CHECK_VERSION(3, 98, 4)
static void dim_im_context_set_client_window(GtkIMContext *context, GdkWidget *client)
#else
static void dim_im_context_set_client_window(GtkIMContext *context, GdkWindow *client)
#endif
{
    g_return_if_fail(GTK_IS_IM_CONTEXT(context));
    g_return_if_fail(client);

    DimIMContext *context_wayland = DIM_IM_CONTEXT(context);

    if (client == context_wayland->window)
        return;

    if (context_wayland->window)
        dim_im_context_focus_out(context);

    g_set_object(&context_wayland->window, client);
}

static gboolean dim_im_context_filter_keypress(GtkIMContext *context, GdkEventKey *event)
{
    /* This is done by the compositor */
    return GTK_IM_CONTEXT_CLASS(dim_im_context_parent_class)->filter_keypress(context, event);
}

static void dim_im_context_focus_in(GtkIMContext *context)
{
    g_return_if_fail(GTK_IS_IM_CONTEXT(context));

    DimIMContext *self = DIM_IM_CONTEXT(context);

    if (self->window == nullptr)
        return;

    DimIMContextWaylandGlobal *global =
        dim_im_context_wayland_global_get(gdk_window_get_display(self->window));
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

static void dim_im_context_focus_out(GtkIMContext *context)
{
    DimIMContext *self = DIM_IM_CONTEXT(context);

    DimIMContextWaylandGlobal *global = dim_im_context_wayland_get_global(self);
    if (global == nullptr)
        return;

    disable(self, global);

    global->current = nullptr;
}

static void dim_im_context_reset(GtkIMContext *context)
{
    DimIMContext *self = DIM_IM_CONTEXT(context);
    DimIMContextWaylandGlobal *global = dim_im_context_wayland_get_global(self);
    if (global == nullptr)
        return;
    notify_im_change(DIM_IM_CONTEXT(context), ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_OTHER);
}

static void dim_im_context_wayland_commit(GtkIMContext *context, const gchar *str)
{
    if (GTK_IM_CONTEXT_CLASS(dim_im_context_parent_class)->commit)
        GTK_IM_CONTEXT_CLASS(dim_im_context_parent_class)->commit(context, str);

    notify_im_change(DIM_IM_CONTEXT(context), ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_INPUT_METHOD);
}

static void dim_im_context_set_cursor_location(GtkIMContext *context, GdkRectangle *area)
{
    DimIMContext *context_wayland = DIM_IM_CONTEXT(context);

    if (context_wayland->cursor_rect.x == area->x && context_wayland->cursor_rect.y == area->y
        && context_wayland->cursor_rect.width == area->width
        && context_wayland->cursor_rect.height == area->height)
        return;

    int side = context_wayland->cursor_rect.height;
    context_wayland->cursor_rect = *area;
}

static void dim_im_context_set_use_preedit(GtkIMContext *context, gboolean use_preedit)
{
    DimIMContext *context_wayland = DIM_IM_CONTEXT(context);

    context_wayland->use_preedit = !!use_preedit;
}

static void dim_im_context_set_surrounding(GtkIMContext *context,
                                           const gchar *text,
                                           gint len,
                                           gint cursor_index)
{
    DimIMContext *context_wayland = DIM_IM_CONTEXT(context);

    if (context_wayland->surrounding.text && text
        && (len < 0 || len == strlen(context_wayland->surrounding.text))
        && strncmp(context_wayland->surrounding.text, text, len) == 0
        && context_wayland->surrounding.cursor_idx == cursor_index)
        return;

    g_free(context_wayland->surrounding.text);
    context_wayland->surrounding.text = g_strndup(text, len);
    context_wayland->surrounding.cursor_idx = cursor_index;
}

static void dim_im_context_get_preedit_string(GtkIMContext *context,
                                              gchar **str,
                                              PangoAttrList **attrs,
                                              gint *cursor_pos)
{
    DimIMContext *context_wayland = DIM_IM_CONTEXT(context);

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
        context_wayland->current_preedit.text ? context_wayland->current_preedit.text : "";

    if (cursor_pos)
        *cursor_pos = g_utf8_strlen(preedit_str, context_wayland->current_preedit.cursor_begin);

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

        if (context_wayland->current_preedit.cursor_begin
            != context_wayland->current_preedit.cursor_end) {
            /* FIXME: Oh noes, how to highlight while taking into account user preferences? */
            PangoAttribute *cursor = pango_attr_weight_new(PANGO_WEIGHT_BOLD);
            cursor->start_index = context_wayland->current_preedit.cursor_begin;
            cursor->end_index = context_wayland->current_preedit.cursor_end;
            pango_attr_list_insert(*attrs, cursor);
        }
    }
}

static gboolean dim_im_context_get_surrounding(GtkIMContext *context,
                                               gchar **text,
                                               gint *cursor_index)
{
    DimIMContext *context_wayland = DIM_IM_CONTEXT(context);

    if (!context_wayland->surrounding.text)
        return FALSE;

    *text = context_wayland->surrounding.text;
    *cursor_index = context_wayland->surrounding.cursor_idx;
    return TRUE;
}

static void dim_im_context_finalize(GObject *obj)
{
    DimIMContext *context = DIM_IM_CONTEXT(obj);

    dim_im_context_focus_out(GTK_IM_CONTEXT(context));

    g_clear_object(&context->window);
    g_free(context->surrounding.text);
    g_free(context->current_preedit.text);
    g_free(context->pending_preedit.text);
    g_free(context->pending_commit);

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
    im_context_class->set_client_window = dim_im_context_set_client_window;
#endif
    im_context_class->get_preedit_string = dim_im_context_get_preedit_string;
    im_context_class->filter_keypress = dim_im_context_filter_keypress;
    im_context_class->focus_in = dim_im_context_focus_in;
    im_context_class->focus_out = dim_im_context_focus_out;
    im_context_class->reset = dim_im_context_reset;
    im_context_class->commit = dim_im_context_wayland_commit;
    im_context_class->set_cursor_location = dim_im_context_set_cursor_location;
    im_context_class->set_use_preedit = dim_im_context_set_use_preedit;
    im_context_class->set_surrounding = dim_im_context_set_surrounding;
    im_context_class->get_surrounding = dim_im_context_get_surrounding;

    gobject_class->finalize = dim_im_context_finalize;
}

static void dim_im_context_class_finalize(DimIMContextClass *klass) { }

static void on_content_type_changed(DimIMContext *context)
{
    notify_content_type(context);
    commit_state(context);
}

static void dim_im_context_init(DimIMContext *context)
{
    context->use_preedit = TRUE;

    g_signal_connect_swapped(context,
                             "notify::input-purpose",
                             G_CALLBACK(on_content_type_changed),
                             context);
    g_signal_connect_swapped(context,
                             "notify::input-hints",
                             G_CALLBACK(on_content_type_changed),
                             context);
}

DimIMContext *dim_im_context_new(void)
{
    GObject *obj = (GObject *)g_object_new(DIM_TYPE_IM_CONTEXT, nullptr);
    return DIM_IM_CONTEXT(obj);
}

void dim_im_context_register(GTypeModule *type_module)
{
    dim_im_context_register_type(type_module);
}
