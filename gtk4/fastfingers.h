#pragma once
#ifndef FF_APPLICATION
#define FF_APPLICATION

#include "home-page.h"
#include "card.h"
#include "practice-page.h"
#include "ff-utils.h"
#include "settings-page.h"
#include "credits-page.h"
#include "reset-progress-page.h"

G_BEGIN_DECLS

void
activate (GtkApplication *app,
          gpointer        user_data);

void
ff_switch_page (const char *page);

void
ff_switch_previous (void);

const char*
ff_get_previous_page (void);

const char*
ff_get_current_page (void);

GtkWidget *ff_get_stack (void);

GtkWidget *ff_get_window (void);
  
G_END_DECLS

#endif
