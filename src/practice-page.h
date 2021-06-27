#pragma once
#ifndef FF_PRACTICE
#define FF_PRACTICE

#include "key.h"
#include "ff-utils.h"
#include "fastfingers.h"

G_BEGIN_DECLS

void
ff_practice_page_init(GtkStack *stack, cJSON *app, const char *category);
  
gboolean
key_pressed_cb (GtkEventControllerKey *controller,
		guint keyval,
		guint keycode,
		GdkModifierType state,
		gpointer user_data);

G_END_DECLS

#endif
