// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef _DIM_IM_CONTEXT_H
#define _DIM_IM_CONTEXT_H

#include <gtk/gtk.h>

/*
 * Type macros.
 */
#define DIM_TYPE_IM_CONTEXT (dim_im_context_get_type())
#define DIM_IM_CONTEXT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), DIM_TYPE_IM_CONTEXT, DimIMContext))

typedef struct _DimIMContextWaylandGlobal DimIMContextWaylandGlobal;
typedef struct _DimIMContext DimIMContext;
typedef struct _DimIMContextClass DimIMContextClass;

struct _DimIMContextClass
{
    GtkIMContextClass parent_class;
};

G_BEGIN_DECLS
GType dim_im_context_get_type(void) G_GNUC_CONST;
DimIMContext *dimImContextNew(void);
void dim_im_context_register(GTypeModule *type_module);
G_END_DECLS

#endif // !_DIM_IM_CONTEXT_H
