// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "imcontext.h"

#include <gtk/gtk.h>
#include <gtk/gtkimmodule.h>

static const GtkIMContextInfo _info = {
    .context_id = "dim",
    .context_name = "DIM (deepin IM)",
    .domain = "dim",
    .domain_dirname = "/usr/share/locale",
    .default_locales = "zh:jp:ko:*",
};

static const GtkIMContextInfo *_infos[] = { &_info };

extern "C" {

G_MODULE_EXPORT const gchar *g_module_check_init(G_GNUC_UNUSED GModule *module)
{
    return glib_check_version(GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION, 0);
}

G_MODULE_EXPORT void im_module_list(const GtkIMContextInfo ***contexts, guint *n_contexts)
{
    *contexts = _infos;
    *n_contexts = G_N_ELEMENTS(_infos);
}

G_MODULE_EXPORT void im_module_init(GTypeModule *type_module)
{
    g_type_module_use(type_module);
    dim_im_context_register_type(type_module);
}

G_MODULE_EXPORT void im_module_exit(void) { }

G_MODULE_EXPORT GtkIMContext *im_module_create(const gchar *context_id)
{
    if (g_strcmp0(context_id, _info.context_id) != 0) {
        return nullptr;
    }

    auto gty = get_im_context_gtype();
    if (gty == 0) {
        return nullptr;
    }

    gpointer obj = g_object_new(gty, nullptr);
    return GTK_IM_CONTEXT(obj);
}
}
