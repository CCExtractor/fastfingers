#pragma once
#ifndef _FF_LIB
#define _FF_LIB

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "cJSON.h"

G_BEGIN_DECLS

typedef struct _resizable_container FFResizableContainer;

void
ff_init_css (void);
  
char *
ff_simplify_title (const char *title);
  
char *
ff_logo_path_gen (const char *title);

gboolean ff_resize_image(GtkWidget *widget,
			 GdkRectangle *allocation,
			 gpointer user_data);

G_END_DECLS

#endif
