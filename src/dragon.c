/* dragon - very lightweight DnD file source/target
** Copyright 2014 Michael Homer.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "globals.h"
#include "uri.h"
#include "button.h"

char** uri_collection = NULL;
int uri_count = 0;
bool drag_all = false;

GtkWidget *window = NULL;
GtkWidget *vbox = NULL;
GtkIconTheme *icon_theme = NULL;

bool verbose = false;
int mode = 0;
bool and_exit = false;
bool keep = false;
bool print_path = false;

void target_mode()
{
    add_target_button();
    gtk_widget_show_all(window);
    gtk_main();
}

void do_quit(GtkWidget *widget, gpointer data) {
    exit(0);
}

int main (int argc, char **argv) {
    char    *progname = argv[0];
    char *filename = NULL;
    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            mode = MODE_HELP;
            printf("dragon - lightweight DnD source/target\n");
            printf("Usage: %s [OPTION] [FILENAME]\n", argv[0]);
            printf("  --and-exit,   -x  exit after a single completed drop\n");
            printf("  --target,     -t  act as a target instead of source\n");
            printf("  --keep,       -k  with --target, keep files to drag out\n");
            printf("  --print-path, -p  with --target, print file paths"
                    " instead of URIs\n");
            printf("  --all,        -a  drag all files at once\n");
            printf("  --verbose,    -v  be verbose\n");
            printf("  --help            show help\n");
            printf("  --version         show version details\n");
            exit(0);
        } else if (strcmp(argv[i], "--version") == 0) {
            mode = MODE_VERSION;
            puts("dragon " VERSION);
            puts("Copyright (C) 2014-2018 Michael Homer");
            puts("This program comes with ABSOLUTELY NO WARRANTY.");
            puts("See the source for copying conditions.");
            exit(0);
        } else if (strcmp(argv[i], "-v") == 0
                || strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        } else if (strcmp(argv[i], "-t") == 0
                || strcmp(argv[i], "--target") == 0) {
            mode = MODE_TARGET;
        } else if (strcmp(argv[i], "-x") == 0
                || strcmp(argv[i], "--and-exit") == 0) {
            and_exit = true;
        } else if (strcmp(argv[i], "-k") == 0
                || strcmp(argv[i], "--keep") == 0) {
            keep = true;
        } else if (strcmp(argv[i], "-p") == 0
                || strcmp(argv[i], "--print-path") == 0) {
            print_path = true;
        } else if (strcmp(argv[i], "-a") == 0
                || strcmp(argv[i], "--all") == 0) {
            drag_all = true;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "%s: error: unknown option `%s'.\n",
                    progname, argv[i]);
        }
    }
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);

    GtkAccelGroup *accelgroup;
    GClosure *closure;

    gtk_init(&argc, &argv);

    icon_theme = gtk_icon_theme_get_default();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    closure = g_cclosure_new(G_CALLBACK(do_quit), NULL, NULL);
    accelgroup = gtk_accel_group_new();
    gtk_accel_group_connect(accelgroup, GDK_KEY_Escape, 0, 0, closure);
    gtk_window_add_accel_group(GTK_WINDOW(window), accelgroup);

    gtk_window_set_title(GTK_WINDOW(window), "Run");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_window_set_title(GTK_WINDOW(window), "dragon");

    if (mode == MODE_TARGET) {
            target_mode();
            exit(0);
    }

    if(drag_all){
       uri_collection = malloc(sizeof(char*) * (argc > MAX_SIZE? argc: MAX_SIZE));
       uri_count = 0;
    }

    bool had_filename = false;
    for (int i=1; i<argc; i++) {
        if (argv[i][0] != '-') {
            filename = argv[i];
            if (!is_uri(filename)) {
                add_filename_button(filename);
            } else if (is_file_uri(filename)) {
                GFile *file = g_file_new_for_uri(filename);
                add_file_button(file);
            } else {
                add_uri_button(filename);
            }
            had_filename = true;
        }
    }
    if (!had_filename) {
        printf("Usage: %s [OPTIONS] FILENAME\n", progname);
        exit(0);
    }

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
