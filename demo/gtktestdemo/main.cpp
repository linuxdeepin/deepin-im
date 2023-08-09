// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtk/gtk.h>

static GtkWidget *entry;

void create_input_box(GtkWidget *w)
{
    GtkWidget *label;
    GtkWidget *main_box;
    GtkWidget *input_box;

    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(w), main_box);

    input_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_hexpand(input_box, TRUE);
    gtk_widget_set_halign(input_box, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(main_box), input_box, FALSE, FALSE, 5);

    label = gtk_label_new("请输入：");
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(input_box), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(input_box), entry, FALSE, FALSE, 5);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_POPUP);

    gtk_window_set_default_size(GTK_WINDOW(window), 100, 80);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    create_input_box(window);
    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();

    return 0;
}