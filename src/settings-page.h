#pragma once
#ifndef _FF_SETTINGS
#define _FF_SETTINGS

#include "key.h"
#include "ff-utils.h"
#include "fastfingers.h"

#include <wordexp.h>
#include <unistd.h>

G_BEGIN_DECLS

void ff_settings_page_init(GtkStack *stack);

G_END_DECLS

#endif
