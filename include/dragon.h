/*
** dragon.h
** File description:
** Header file for dragon.c
*/

#ifndef __DRAGON_H__
# define __DRAGON_H__

#   define _POSIX_C_SOURCE (200809L)
#   define _XOPEN_SOURCE (500)

#   include <gtk/gtk.h>
#   include <gdk/gdk.h>
#   include <gdk/gdkkeysyms.h>
#   include <gio/gio.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <stdbool.h>
#   include <string.h>

#   define VERSION "1.1.0"

#endif /* __DRAGON_H__ */

#ifndef __DG_MODES_H__
# define __DG_MODES_H__

#   define MODE_HELP        (1)
#   define MODE_TARGET      (2)
#   define MODE_VERSION     (4)

#   define TARGET_TYPE_TEXT (1)
#   define TARGET_TYPE_URI  (2)

typedef struct s_draggable {
    char    *text;
    char    *uri;
    guint   last_time;
}   draggable_t;

#endif /* __DG_MODES_H__ */

