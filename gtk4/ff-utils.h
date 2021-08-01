#pragma once
#ifndef _FF_LIB
#define _FF_LIB

#include "cJSON.h"
#include <ctype.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

G_BEGIN_DECLS

void ff_init_css(void);

char *ff_simplify_title(const char *title);

char *ff_logo_path_gen(const char *title);

G_END_DECLS

#endif
