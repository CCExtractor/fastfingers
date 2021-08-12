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

#define GTK_CALLBACK(f) ((GtkCallback) (f))
#ifndef G_SOURCE_FUNC
#define G_SOURCE_FUNC(f) ((GSourceFunc) (void(*)(void))(f))
#endif

typedef struct _resizable_container FFResizableContainer;

typedef struct dynamicArray dynamicArray;

void ff_init_css(void);

char *ff_simplify_title(const char *title);

char *ff_logo_path_gen(const char *title);

gboolean ff_resize_image(GtkWidget *widget, GdkRectangle *allocation,
                         gpointer user_data);

cJSON *ff_get_application(const char *name);

void set_scaled_image(GtkImage *image, const char *title, int size);

int key_compare(guint keyval1, guint keyval2);

guint get_keyval_from_name(const char *str);
char *get_keyval_name(guint keyval);

char *normalize_keyval_name(const char *str);

void null_check(void *check_ptr, const char *str, void (*fptr)(void *),
                void *free_arg);

GtkWidget *ff_box_nth_child(GtkWidget *box, int idx);
GtkWidget *ff_box_first_child(GtkWidget *box);
GtkWidget *ff_box_last_child(GtkWidget *box);

void add_style_class(GtkWidget *widget, const char *class);
void remove_style_class(GtkWidget *widget, const char *class);

void ff_container_remove_all(GtkWidget *container);

dynamicArray *ff_dynamicArray_new(size_t arraySize, size_t itemSize);
size_t ff_dynamicArray_append(dynamicArray *arr, const void *item);
size_t ff_dynamicArray_prepend(dynamicArray *arr, const void *item);
void *ff_dynamicArray_get(dynamicArray *arr, size_t idx);
size_t ff_dynamicArray_size(dynamicArray *arr);
void ff_dynamicArray_free(dynamicArray *arr);

G_END_DECLS

#endif
