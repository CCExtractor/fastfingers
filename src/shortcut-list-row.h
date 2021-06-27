#pragma once
#ifndef _FF_SHORTCUT_LIST_ROW
#define _FF_SHORTCUT_LIST_ROW

#include "ff-utils.h"

G_BEGIN_DECLS

#define FF_TYPE_SHORTCUT_LIST_ROW ff_shortcut_list_row_get_type ()

G_DECLARE_FINAL_TYPE (FFShortcutListRow, ff_shortcut_list_row, FF, SHORTCUT_LIST_ROW, GtkListBoxRow)

GtkWidget*
ff_shortcut_list_row_new (const gchar *left, const gchar *right);

const gchar *
ff_shortcut_list_row_get_left_text (FFShortcutListRow *row);

void
ff_shortcut_list_row_set_left_text (FFShortcutListRow *row,
				    const gchar* text);

const gchar *
ff_shortcut_list_row_get_right_text (FFShortcutListRow *row);

void
ff_shortcut_list_row_set_right_text (FFShortcutListRow *row,
				     const gchar* text);

G_END_DECLS

#endif
