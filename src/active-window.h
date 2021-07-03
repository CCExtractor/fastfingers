#pragma once
#ifndef _FF_ACTIVE_WINDOW
#define _FF_ACTIVE_WINDOW
 
#include "ff-utils.h"

#include <gio/gio.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _X11_FOUND
  char *get_active_from_xorg (void);
#endif

char *get_active_from_gnome_shell (void);

#endif
