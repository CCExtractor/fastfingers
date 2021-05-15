#pragma once
#ifndef _FF_LIB
#define _FF_LIB

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

char *
ff_logo_path_gen (const char *title);

extern GObject *stack;
extern GObject *window;

G_END_DECLS

#endif
