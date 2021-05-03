#ifndef FF_PRACTICE
#define FF_PRACTICE

#include "ff_utils.h"

GtkWidget *ff_new_practice_page(void);

static GtkWidget *
key_add(GtkBox *parent_box,
	const char *key_description,  const char *key_letter);
  
gboolean
on_key_press (GtkEventControllerKey *controller,
	      guint keyval,
	      guint keycode,
	      GdkModifierType state,
	      gpointer user_data);

#endif
