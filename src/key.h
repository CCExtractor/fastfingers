#pragma once
#ifndef _FF_KEY
#define _FF_KEY

#include "ff-utils.h"

G_BEGIN_DECLS

#define FF_TYPE_KEY ff_key_get_type()

G_DECLARE_FINAL_TYPE(FFKey, ff_key, FF, KEY, GtkLabel)

GtkWidget *ff_key_new(const gchar *text, int is_visible);
void ff_key_set_style(GtkWidget *key, const char *style);

const gchar *ff_key_get_text(FFKey *key);
void ff_key_set_text(FFKey *key, const gchar *text);
int ff_key_get_visible(FFKey *key);
void ff_key_set_visible(FFKey *key, int state);

G_END_DECLS

#endif
