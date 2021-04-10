#ifndef CARDS
#define CARDS

#include "fflib.h"

GtkWidget*
attach_new_card (GtkGrid *grid, int i_top,
		 const char *title,
		 int completed, int total);

#endif
