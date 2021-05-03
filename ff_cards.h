#ifndef FF_CARDS
#define FF_CARDS

#include "ff_utils.h"
#include "ff_practice.h"

GtkWidget*
attach_new_card (GtkGrid *grid, int i_top,
		 const char *title,
		 int completed, int total);

static void
on_click (GtkGestureClick *gesture,
	  gint             n_press,
	  gdouble          x,
	  gdouble          y,
	  GtkWidget   *card);
  
#endif
