/*
** drag.h
** File description:
** Header file for 
*/

#ifndef __DRAG_H__
# define __DRAG_H__

#include "dragon.h"

void drag_data_received(GtkWidget          *widget,
                        GdkDragContext     *context,
                        gint                x,
                        gint                y,
                        GtkSelectionData   *data,
                        guint               info,
                        guint               time);
void drag_end(GtkWidget *widget, GdkDragContext *context, gpointer user_data);
void drag_data_get(GtkWidget    *widget,
                   GdkDragContext   *context,
                   GtkSelectionData *data,
                   guint             info,
                   guint             time,
                   gpointer          user_data);
gboolean drag_drop(GtkWidget *widget,
                   GdkDragContext *context,
                   gint            x,
                   gint            y,
                   guint           time,
                   gpointer        user_data);

#endif /* __DRAG_H__ */
