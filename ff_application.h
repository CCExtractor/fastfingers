#ifndef FF_APPLICATION
#define FF_APPLICATION

#include "ff_home.h"
#include "ff_cards.h"
#include "ff_practice.h"
#include "ff_utils.h"

void
activate (GtkApplication *app,
          gpointer        user_data);

static void
ff_init_css (void);

#endif
