#pragma once
#ifndef _FF_KEY
#define _FF_KEY

#include "ff-utils.h"

G_BEGIN_DECLS

#define FF_TYPE_KEY ff_key_get_type()

G_DECLARE_FINAL_TYPE(FFKey, ff_key, FF, KEY, GtkLabel)

GtkWidget *ff_key_new(const gchar *text);

G_END_DECLS

#endif
