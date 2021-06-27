#pragma once
#ifndef _FF_CARD
#define _FF_CARD

#include "ff-utils.h"
#include "application-page.h"
#include "fastfingers.h"

G_BEGIN_DECLS

#define FF_TYPE_CARD ff_card_get_type ()

G_DECLARE_FINAL_TYPE (FFCard, ff_card, FF, CARD, GtkEventBox)

GtkWidget*
ff_card_new (const gchar *title, double progress);

static void
card_click_cb (FFCard    *card,
	       GdkEvent  *event,
	       gpointer   user_data);

G_END_DECLS

#endif
