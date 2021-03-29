#ifndef FFLIB
#define FFLIB

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#define FF_HOME 1
#define FF_LIST 2
#define FF_PRACTICE 3

void logo_path_gen (char *logo_path, const char *title);

void ff_init_css (void);

extern GtkWidget *stack;
extern GtkWidget *window;

extern int page;

#endif
