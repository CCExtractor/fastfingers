#pragma once
#ifndef FF_QUIZ
#define FF_QUIZ

#include "fastfingers.h"
#include "ff-utils.h"
#include "quiz-result-page.h"
#include "key.h"

G_BEGIN_DECLS

void ff_quiz_page_init(GtkStack *stack, cJSON *app, dynamicArray *darray);

G_END_DECLS

#endif
