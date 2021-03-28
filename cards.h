#ifndef CARDS
#define CARDS

#include "fflib.h"

void sketch(void);
void vscode(void);
void gmail(void);
void slack(void);

GtkWidget*
attach_new_card (GtkGrid *grid, int i_top,
		 const char *title,
		 int completed, int total,
		 void (*callback_func)(void));

#endif
