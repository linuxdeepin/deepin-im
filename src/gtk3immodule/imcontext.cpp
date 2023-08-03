#include "imcontext.h"

#include <gtk/gtkimmodule.h>

static GType _im_context_gtype = 0;

class signals
{
public:
    static guint commit;
    static guint preedit_changed;
    static guint preedit_start;
    static guint preedit_end;
};

guint signals::commit = 0;
guint signals::preedit_changed = 0;
guint signals::preedit_start = 0;
guint signals::preedit_end = 0;

void im_context_finalize(GObject *obj);

// virtual functions start

#if GTK_CHECK_VERSION(3, 98, 4)
void set_client(GtkIMContext *context, GdkWidget *client)
#else
void set_client(GtkIMContext *context, GdkWindow *client)
#endif
{
}

void get_preedit_string(GtkIMContext *context, gchar **str, PangoAttrList **attrs, gint *cursor_pos)
{
}

gboolean filter_keypress(GtkIMContext *context, GdkEventKey *event)
{
    return false;
}

void focus_in(GtkIMContext *context) { }

void focus_out(GtkIMContext *context) { }

void reset(GtkIMContext *context) { }

void set_cursor_location(GtkIMContext *context, GdkRectangle *area) { }

void set_use_preedit(GtkIMContext *context, gboolean use_preedit) { }

void set_surrounding(GtkIMContext *context, const gchar *text, gint len, gint cursor_index) { }

gboolean get_surrounding(GtkIMContext *context, gchar **text, gint *cursor_index)
{
    return false;
}

// virtual functions end

struct DimIMContextClass
{
    GtkIMContextClass parent;
};

void im_context_class_init(DimIMContextClass *klass, gpointer _data)
{
    signals::commit = g_signal_lookup("commit", G_TYPE_FROM_CLASS(klass));
    signals::preedit_changed = g_signal_lookup("preedit-changed", G_TYPE_FROM_CLASS(klass));
    signals::preedit_start = g_signal_lookup("preedit-start", G_TYPE_FROM_CLASS(klass));
    signals::preedit_end = g_signal_lookup("preedit-end", G_TYPE_FROM_CLASS(klass));

#if GTK_CHECK_VERSION(3, 98, 4)
    klass->parent.set_client_widget = set_client;
#else
    klass->parent.set_client_window = set_client;
#endif
    klass->parent.get_preedit_string = get_preedit_string;
    klass->parent.filter_keypress = filter_keypress;
    klass->parent.focus_in = focus_in;
    klass->parent.focus_out = focus_out;
    klass->parent.reset = reset;
    klass->parent.set_cursor_location = set_cursor_location;
    klass->parent.set_use_preedit = set_use_preedit;
    klass->parent.set_surrounding = set_surrounding;
    klass->parent.get_surrounding = get_surrounding;

    GObjectClass *parent_class = G_OBJECT_CLASS(klass);
    parent_class->finalize = im_context_finalize;
}

void im_context_class_finalize(DimIMContextClass *klass, gpointer _data) { }

struct DimIMContext
{
    GtkIMContext parent;
};

void im_context_init(DimIMContext *ctx, DimIMContextClass *klass) { }

void im_context_finalize(GObject *obj) { }

GType get_im_context_gtype()
{
    return _im_context_gtype;
}

void dim_im_context_register_type(GTypeModule *type_module)
{
    static const GTypeInfo type_info = {
        .class_size = sizeof(DimIMContextClass),
        .base_init = nullptr,
        .base_finalize = nullptr,
        .class_init = (GClassInitFunc)im_context_class_init,
        .class_finalize = (GClassFinalizeFunc)im_context_class_finalize,
        .class_data = nullptr,
        .instance_size = sizeof(DimIMContext),
        .instance_init = (GInstanceInitFunc)im_context_init,
        .value_table = nullptr,
    };

    if (type_module) {
        _im_context_gtype = g_type_module_register_type(type_module,
                                                        gtk_im_context_get_type(),
                                                        "DimIMContext",
                                                        &type_info,
                                                        G_TYPE_FLAG_NONE);
    } else {
        _im_context_gtype = g_type_register_static(gtk_im_context_get_type(),
                                                   "DimIMContext",
                                                   &type_info,
                                                   G_TYPE_FLAG_NONE);
    }
}
