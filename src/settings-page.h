#pragma once
#ifndef _FF_SETTINGS
#define _FF_SETTINGS

#include "key.h"
#include "ff-utils.h"
#include "fastfingers.h"

G_BEGIN_DECLS

void ff_settings_page_init(GtkStack *stack);

void
settings_cb (GtkGestureClick *gesture,
	     gint             n_press,
	     gdouble          x,
	     gdouble          y,
	     GtkWidget   *widget);

G_END_DECLS

#endif
