#pragma once
#ifndef FF_PRACTICE
#define FF_PRACTICE

#include "fastfingers.h"
#include "ff-utils.h"
#include "key.h"

G_BEGIN_DECLS

void ff_practice_page_init(GtkStack *stack, cJSON *app, const char *category);

G_END_DECLS

#endif
