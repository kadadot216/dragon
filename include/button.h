/*
** button.h
** File description:
** Header file for button.c
*/

#ifndef __BUTTON_H__
# define __BUTTON_H__

#include "dragon.h"

void add_file_button(GFile *file);
void add_filename_button(char *filename);
void add_uri_button(char *uri);
void add_target_button();
void button_clicked(GtkWidget *widget, gpointer user_data);
void left_align_button(GtkButton *button);
GtkButton *add_button(char *label, draggable_t *dragdata, int type);
GtkIconInfo* icon_info_from_content_type(char *content_type);

#endif /* __BUTTON_H__ */
