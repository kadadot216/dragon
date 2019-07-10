/*
** button.c
** File description:
** Button related functions
*/

#include "globals.h"
#include "drag.h"

void button_clicked(GtkWidget *widget, gpointer user_data) {
    draggable_t *dd = (draggable_t *)user_data;
    if (0 == fork()) {
        execlp("xdg-open", "xdg-open", dd->uri, NULL);
    }
}

GtkButton *add_button(char *label, draggable_t *dragdata, int type) {
    GtkWidget *button = gtk_button_new_with_label(label);

    GtkTargetList *targetlist = gtk_drag_source_get_target_list(GTK_WIDGET(button));
    if (targetlist)
        gtk_target_list_ref(targetlist);
    else
        targetlist = gtk_target_list_new(NULL, 0);
    if (type == TARGET_TYPE_URI)
        gtk_target_list_add_uri_targets(targetlist, TARGET_TYPE_URI);
    else
        gtk_target_list_add_text_targets(targetlist, TARGET_TYPE_TEXT);
    gtk_drag_source_set(GTK_WIDGET(button), GDK_BUTTON1_MASK, NULL, 0,
            GDK_ACTION_DEFAULT | GDK_ACTION_LINK | GDK_ACTION_COPY);
    gtk_drag_source_set_target_list(GTK_WIDGET(button), targetlist);
    g_signal_connect(GTK_WIDGET(button), "drag-data-get",
            G_CALLBACK(drag_data_get), dragdata);
    g_signal_connect(GTK_WIDGET(button), "clicked",
            G_CALLBACK(button_clicked), dragdata);
    g_signal_connect(GTK_WIDGET(button), "drag-end",
            G_CALLBACK(drag_end), dragdata);

    gtk_container_add(GTK_CONTAINER(vbox), button);

    if(drag_all)
        uri_collection[uri_count++] = dragdata->uri;

    return (GtkButton *)button;
}

void left_align_button(GtkButton *button) {
    GList *child = g_list_first(
            gtk_container_get_children(GTK_CONTAINER(button)));
    if (child)
        gtk_widget_set_halign(GTK_WIDGET(child->data), GTK_ALIGN_START);
}

GtkIconInfo* icon_info_from_content_type(char *content_type) {
    GIcon *icon = g_content_type_get_icon(content_type);
    return gtk_icon_theme_lookup_by_gicon(icon_theme, icon, 48, 0);
}

void add_file_button(GFile *file) {
    char *filename = g_file_get_path(file);
    if(!g_file_query_exists(file, NULL)) {
        fprintf(stderr, "The file `%s' does not exist.\n",
                filename);
        exit(1);
    }
    char *uri = g_file_get_uri(file);
    draggable_t *dragdata = malloc(sizeof(draggable_t));
    dragdata->text = filename;
    dragdata->uri = uri;

    GtkButton *button = add_button(filename, dragdata, TARGET_TYPE_URI);
    GFileInfo *fileinfo = g_file_query_info(file, "*", 0, NULL, NULL);
    GIcon *icon = g_file_info_get_icon(fileinfo);
    GtkIconInfo *icon_info = gtk_icon_theme_lookup_by_gicon(icon_theme,
            icon, 48, 0);

    // Try a few fallback mimetypes if no icon can be found
    if (!icon_info)
        icon_info = icon_info_from_content_type("application/octet-stream");
    if (!icon_info)
        icon_info = icon_info_from_content_type("text/x-generic");
    if (!icon_info)
        icon_info = icon_info_from_content_type("text/plain");

    if (icon_info) {
        GtkWidget *image = gtk_image_new_from_pixbuf(
                gtk_icon_info_load_icon(icon_info, NULL));
        gtk_button_set_image(button, image);
        gtk_button_set_always_show_image(button, true);
    }

    left_align_button(button);
}

void add_filename_button(char *filename) {
    GFile *file = g_file_new_for_path(filename);
    add_file_button(file);
}

void add_uri_button(char *uri) {
    draggable_t *dragdata = malloc(sizeof(draggable_t));
    dragdata->text = uri;
    dragdata->uri = uri;
    GtkButton *button = add_button(uri, dragdata, TARGET_TYPE_URI);
    left_align_button(button);
}

void add_target_button()
{
    GtkWidget *label = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(label), "Drag something here...");
    gtk_container_add(GTK_CONTAINER(vbox), label);
    GtkTargetList *targetlist = gtk_drag_dest_get_target_list(GTK_WIDGET(label));
    if (targetlist)
        gtk_target_list_ref(targetlist);
    else
        targetlist = gtk_target_list_new(NULL, 0);
    gtk_target_list_add_text_targets(targetlist, TARGET_TYPE_TEXT);
    gtk_target_list_add_uri_targets(targetlist, TARGET_TYPE_URI);
    gtk_drag_dest_set(GTK_WIDGET(label),
            GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT, NULL, 0,
            GDK_ACTION_COPY);
    gtk_drag_dest_set_target_list(GTK_WIDGET(label), targetlist);
    g_signal_connect(GTK_WIDGET(label), "drag-drop",
            G_CALLBACK(drag_drop), NULL);
    g_signal_connect(GTK_WIDGET(label), "drag-data-received",
            G_CALLBACK(drag_data_received), NULL);
}

