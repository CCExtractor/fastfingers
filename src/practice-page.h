#pragma once
#ifndef FF_PRACTICE
#define FF_PRACTICE

#include "key.h"
#include "ff-utils.h"
#include "fastfingers.h"

G_BEGIN_DECLS

void
ff_practice_page_init(GtkStack *stack, const char *title);

static GtkWidget *
key_add(GtkBox *parent_box,
	const char *key_description,  const char *key_letter);
  
gboolean
on_key_press (GtkEventControllerKey *controller,
	      guint keyval,
	      guint keycode,
	      GdkModifierType state,
	      gpointer user_data);

G_END_DECLS

#endif
