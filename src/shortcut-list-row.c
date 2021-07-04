#include "shortcut-list-row.h"

struct _FFShortcutListRow {
  GtkListBoxRow parent;

  GtkBox *box;
  GtkLabel *left_label;
  GtkLabel *right_label;
};

G_DEFINE_TYPE(FFShortcutListRow, ff_shortcut_list_row, GTK_TYPE_LIST_BOX_ROW);

static void ff_shortcut_list_row_dispose(GObject *object) {
  G_OBJECT_CLASS(ff_shortcut_list_row_parent_class)->dispose(object);
}

const gchar *ff_shortcut_list_row_get_left_text(FFShortcutListRow *row) {
  g_return_val_if_fail(FF_IS_SHORTCUT_LIST_ROW(row), NULL);

  return gtk_label_get_text(row->left_label);
}

void ff_shortcut_list_row_set_left_text(FFShortcutListRow *row,
                                        const gchar *text) {
  g_return_if_fail(FF_IS_SHORTCUT_LIST_ROW(row));

  if (!strcmp(text, ff_shortcut_list_row_get_left_text(row)))
    return;

  gtk_label_set_text(row->left_label, text);
}

const gchar *ff_shortcut_list_row_get_right_text(FFShortcutListRow *row) {
  g_return_val_if_fail(FF_IS_SHORTCUT_LIST_ROW(row), NULL);

  return gtk_label_get_text(row->right_label);
}

void ff_shortcut_list_row_set_right_text(FFShortcutListRow *row,
                                         const gchar *text) {
  g_return_if_fail(FF_IS_SHORTCUT_LIST_ROW(row));

  if (!strcmp(text, ff_shortcut_list_row_get_right_text(row)))
    return;

  gtk_label_set_text(row->right_label, text);
}

enum { PROP_0, PROP_LEFT_TEXT, PROP_RIGHT_TEXT, LAST_PROP };

static GParamSpec *props[LAST_PROP] = {
    NULL,
};

static void ff_shortcut_list_row_set_property(GObject *object, guint prop_id,
                                              const GValue *value,
                                              GParamSpec *pspec) {
  FFShortcutListRow *row = FF_SHORTCUT_LIST_ROW(object);

  switch (prop_id) {
  case PROP_LEFT_TEXT:
    ff_shortcut_list_row_set_left_text(row, g_value_get_string(value));
    break;
  case PROP_RIGHT_TEXT:
    ff_shortcut_list_row_set_right_text(row, g_value_get_string(value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void ff_shortcut_list_row_get_property(GObject *object, guint prop_id,
                                              GValue *value,
                                              GParamSpec *pspec) {
  FFShortcutListRow *row = FF_SHORTCUT_LIST_ROW(object);

  switch (prop_id) {
  case PROP_LEFT_TEXT:
    g_value_set_string(value, ff_shortcut_list_row_get_left_text(row));
    break;
  case PROP_RIGHT_TEXT:
    g_value_set_string(value, ff_shortcut_list_row_get_right_text(row));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void ff_shortcut_list_row_class_init(FFShortcutListRowClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

  object_class->set_property = ff_shortcut_list_row_set_property;
  object_class->get_property = ff_shortcut_list_row_get_property;
  object_class->dispose = ff_shortcut_list_row_dispose;

  props[PROP_LEFT_TEXT] = g_param_spec_string(
      "left-text", "left-text", "Left text of the row", "unknown category",
      G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_RIGHT_TEXT] =
      g_param_spec_string("right-text", "right-text", "Right text of the row",
                          "0/0", G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties(object_class, LAST_PROP, props);

  gtk_widget_class_set_template_from_resource(
      widget_class, "/org/ccextractor/FastFingers/ui/shortcut-list-row.ui");
  gtk_widget_class_set_css_name(widget_class, "shortcut-list-row");
  gtk_widget_class_bind_template_child(widget_class, FFShortcutListRow, box);
  gtk_widget_class_bind_template_child(widget_class, FFShortcutListRow,
                                       left_label);
  gtk_widget_class_bind_template_child(widget_class, FFShortcutListRow,
                                       right_label);
}

static void ff_shortcut_list_row_init(FFShortcutListRow *self) {
  gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget *ff_shortcut_list_row_new(const gchar *left, const gchar *right) {
  GtkWidget *_row = g_object_new(FF_TYPE_SHORTCUT_LIST_ROW, "left-text", left,
                                 "right-text", right, NULL);
  return _row;
}
