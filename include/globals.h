/*
** globals.h
** File description:
** Header file for global variables
*/

#ifndef __GLOBALS_H__
# define __GLOBALS_H__

#include "dragon.h"

/* MODE_ALL */
#   define MAX_SIZE (100)

extern char** uri_collection;
extern int uri_count;
extern bool drag_all;
/* */

extern GtkWidget *window;
extern GtkWidget *vbox;
extern GtkIconTheme *icon_theme;

extern bool verbose;
extern int mode;
extern bool and_exit;
extern bool keep;
extern bool print_path;

#endif /* __GLOBALS_H__ */
