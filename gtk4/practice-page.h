#pragma once
#ifndef FF_PRACTICE
#define FF_PRACTICE

#include "fastfingers.h"
#include "ff-utils.h"
#include "key.h"

G_BEGIN_DECLS

void ff_practice_page_init(GtkStack *stack, cJSON *app, const char *category);

static GtkWidget *key_add(GtkBox *parent_box, const char *key_description,
                          const char *key_letter);

gboolean key_pressed_cb(GtkEventControllerKey *controller, guint keyval,
                        guint keycode, GdkModifierType state,
                        gpointer user_data);

G_END_DECLS

#endif
