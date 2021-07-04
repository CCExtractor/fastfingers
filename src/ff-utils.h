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

typedef struct _resizable_container FFResizableContainer;

void ff_init_css(void);

char *ff_simplify_title(const char *title);

char *ff_logo_path_gen(const char *title);

gboolean ff_resize_image(GtkWidget *widget, GdkRectangle *allocation,
                         gpointer user_data);

cJSON *ff_get_application(const char *name);

void set_scaled_image(GtkImage *image, const char *title, int size);

G_END_DECLS

#endif
