#pragma once
#ifndef FF_CARDS
#define FF_CARDS

#include "ff-utils.h"
#include "application-page.h"
#include "fastfingers.h"

G_BEGIN_DECLS

#define FF_TYPE_CARD ff_card_get_type ()

G_DECLARE_FINAL_TYPE (FFCard, ff_card, FF, CARD, GtkBox)

GtkWidget*
ff_card_new (const gchar *title, double progress);

static void
card_click_cb (GtkGestureClick *gesture,
	       gint             n_press,
	       gdouble          x,
	       gdouble          y,
	       GtkWidget   *card);

G_END_DECLS

#endif
