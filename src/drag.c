/*
** drag.c
** File description:
** dragging related functions
*/

#include "globals.h"
#include "uri.h"
#include "button.h"

gboolean drag_drop (GtkWidget *widget,
               GdkDragContext *context,
               gint            x,
               gint            y,
               guint           time,
               gpointer        user_data) {
    GtkTargetList *targetlist = gtk_drag_dest_get_target_list(widget);
    GList *list = gdk_drag_context_list_targets(context);
    if (list) {
        while (list) {
            GdkAtom atom = (GdkAtom)g_list_nth_data(list, 0);
            if (gtk_target_list_find(targetlist,
                        GDK_POINTER_TO_ATOM(g_list_nth_data(list, 0)), NULL)) {
                gtk_drag_get_data(widget, context, atom, time);
                return true;
            }
            list = g_list_next(list);
        }
    }
    gtk_drag_finish(context, false, false, time);
    return true;
}

void
drag_data_received (GtkWidget          *widget,
                    GdkDragContext     *context,
                    gint                x,
                    gint                y,
                    GtkSelectionData   *data,
                    guint               info,
                    guint               time) {
    gchar **uris = gtk_selection_data_get_uris(data);
    unsigned char *text = gtk_selection_data_get_text(data);
    if (!uris && !text)
        gtk_drag_finish (context, FALSE, FALSE, time);
    if (uris) {
        if (verbose)
            fputs("Received URIs\n", stderr);
        gtk_container_remove(GTK_CONTAINER(vbox), widget);
        for (; *uris; uris++) {
            if (is_file_uri(*uris)) {
                GFile *file = g_file_new_for_uri(*uris);
                if (print_path) {
                    char *filename = g_file_get_path(file);
                    printf("%s\n", filename);
                } else
                    printf("%s\n", *uris);
                if (keep)
                    add_file_button(file);

            } else {
                printf("%s\n", *uris);
                if (keep)
                    add_uri_button(*uris);
            }
        }
        add_target_button();
        gtk_widget_show_all(window);
    } else if (text) {
        if (verbose)
            fputs("Received Text\n", stderr);
        printf("%s\n", text);
    } else if (verbose)
        fputs("Received nothing\n", stderr);
    gtk_drag_finish (context, TRUE, FALSE, time);
    if (and_exit)
        gtk_main_quit();
}

void drag_end(GtkWidget *widget, GdkDragContext *context, gpointer user_data) {
    if (and_exit)
        gtk_main_quit();
}

void drag_data_get(GtkWidget    *widget,
               GdkDragContext   *context,
               GtkSelectionData *data,
               guint             info,
               guint             time,
               gpointer          user_data) {
    draggable_t *dd = (draggable_t *)user_data;
    if (info == TARGET_TYPE_URI) {
        if (verbose)
            fprintf(stderr, "Writing as URI: %s\n", dd->uri);

        char** uris;
        if(drag_all){
            uri_collection[uri_count] = NULL;
            uris = uri_collection;
        } else {
            char* a[] = {dd->uri, NULL};
            uris = a;
        }

        gtk_selection_data_set_uris(data, uris);
        g_signal_stop_emission_by_name(widget, "drag-data-get");
    } else if (info == TARGET_TYPE_TEXT) {
        if (verbose)
            fprintf(stderr, "Writing as TEXT: %s\n", dd->text);
        gtk_selection_data_set_text(data, dd->text, -1);
    } else {
        fprintf(stderr, "Error: bad target type %i\n", info);
    }
}
