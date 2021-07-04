#pragma once
#ifndef _FF_BUTTONBOX
#define _FF_BUTTONBOX

#include "fastfingers.h"
#include "ff-utils.h"

G_BEGIN_DECLS

#define FF_TYPE_BUTTONBOX ff_buttonbox_get_type()

G_DECLARE_FINAL_TYPE(FFButtonbox, ff_buttonbox, FF, BUTTONBOX, GtkBox)

GtkWidget *ff_buttonbox_new(void);

const gchar *ff_buttonbox_get_backbutton_label(FFButtonbox *buttonbox);

void ff_buttonbox_set_backbutton_label(FFButtonbox *buttonbox,
                                       const gchar *label);

gboolean ff_buttonbox_get_backbutton_visible(FFButtonbox *buttonbox);

void ff_buttonbox_set_backbutton_visible(FFButtonbox *buttonbox,
                                         gboolean visible);

gboolean ff_buttonbox_get_settings_visible(FFButtonbox *buttonbox);

void ff_buttonbox_set_settings_visible(FFButtonbox *buttonbox,
                                       gboolean visible);

G_END_DECLS

#endif
