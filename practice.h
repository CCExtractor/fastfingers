#ifndef PRACTICE
#define PRACTICE

#include "fflib.h"

GtkWidget *ff_new_practice_page(void);

gboolean
on_key_press (GtkEventControllerKey *controller,
	      guint keyval,
	      guint keycode,
	      GdkModifierType state,
	      gpointer user_data);

#endif
