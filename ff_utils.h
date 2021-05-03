#ifndef FF_LIB
#define FF_LIB

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#define FFP_HOME 1
#define FFP_LIST 2
#define FFP_PRACTICE 3

void ff_logo_path_gen (char *logo_path, const char *title);

extern GtkWidget *stack;
extern GtkWidget *window;

extern int page;

#endif
