#pragma once
#ifndef _FF_LIB
#define _FF_LIB

#include "cJSON.h"
#include <ctype.h>
#include <errno.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <wordexp.h>

G_BEGIN_DECLS

#define GTK_CALLBACK(f) ((GtkCallback)(f))

typedef struct _resizable_container FFResizableContainer;

void ff_init_css(void);

char *ff_simplify_title(const char *title);

char *ff_logo_path_gen(const char *title);

gboolean ff_resize_image(GtkWidget *widget, GdkRectangle *allocation,
                         gpointer user_data);

cJSON *ff_get_application(const char *name);

void set_scaled_image(GtkImage *image, const char *title, int size);

char *normalize_keyval_name(const char *str);

void null_check(void *check_ptr, const char *str, void (*fptr)(void *),
                void *free_arg);

GtkWidget *ff_box_nth_child(GtkWidget *box, int idx);
GtkWidget *ff_box_first_child(GtkWidget *box);
GtkWidget *ff_box_last_child(GtkWidget *box);

void add_style_class(GtkWidget *widget, const char *class);
void remove_style_class(GtkWidget *widget, const char *class);

void ff_container_remove_all(GtkWidget *container);

G_END_DECLS

#endif
