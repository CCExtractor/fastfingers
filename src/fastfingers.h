#pragma once
#ifndef FF_APPLICATION
#define FF_APPLICATION

#include "home-page.h"
#include "card.h"
#include "practice-page.h"
#include "ff-utils.h"
#include "settings-page.h"

G_BEGIN_DECLS

void
activate (GtkApplication *app,
          gpointer        user_data);

static void
ff_init_css (void);

void
ff_switch_page (const char *page);

void
ff_switch_previous (void);

const char*
ff_get_previous_page (void);

const char*
ff_get_current_page (void);

G_END_DECLS

#endif
