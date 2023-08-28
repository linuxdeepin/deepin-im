// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef _DIM_IM_CONTEXT_H
#define _DIM_IM_CONTEXT_H

#include <gtk/gtk.h>
#include "wl/client/ConnectionBase.h"

class DimGtkTextInputV1;

/*
 * Type macros.
 */
#define DIM_TYPE_IM_CONTEXT (dim_im_context_get_type())
#define DIM_IM_CONTEXT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), DIM_TYPE_IM_CONTEXT, DimIMContext))

struct DimIMContextWaylandGlobal
{
    GtkIMContext *current;
    std::shared_ptr<DimGtkTextInputV1> ti;
    wl::client::ConnectionBase *wl;

    guint serial;
    guint doneSerial;
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

struct DimIMContext
{
    GtkIMContextSimple parent;
    GtkIMContext *slave;
    GdkWindow *window;

    struct
    {
        char *text;
        int cursorIdx;
    } surrounding;

    enum zwp_text_input_v3_change_cause surroundingChange;
    surroundingDelete pendingSurroundingDelete;
    preedit currentPreedit;
    preedit pendingPreedit;

    char *pendingCommit;

    cairo_rectangle_int_t cursorRect;
    guint usePreedit : 1;
};

typedef struct _DimIMContextClass DimIMContextClass;

struct _DimIMContextClass
{
    GtkIMContextClass parent_class;
};

void textInputDeleteSurroundingTextApply(DimIMContextWaylandGlobal *global);
void textInputCommitApply(DimIMContextWaylandGlobal *global);
void textInputPreeditApply(DimIMContextWaylandGlobal *global);
void notifyImChange(DimIMContext *context, enum zwp_text_input_v3_change_cause cause);

G_BEGIN_DECLS
GType dim_im_context_get_type(void) G_GNUC_CONST;
DimIMContext *dimImContextNew(void);
void dim_im_context_register(GTypeModule *type_module);
G_END_DECLS

#endif // !_DIM_IM_CONTEXT_H
