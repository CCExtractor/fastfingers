#include "key.h"

struct _FFKey {
  GtkWidget parent;

  GtkLabel *label;
};

G_DEFINE_TYPE(FFKey, ff_key, GTK_TYPE_WIDGET);

static void ff_key_dispose(GObject *object) {
  FFKey *self = FF_KEY(object);

  G_OBJECT_CLASS(ff_key_parent_class)->dispose(object);
}

const gchar *ff_key_get_text(FFKey *key) {
  g_return_val_if_fail(FF_IS_KEY(key), NULL);

  return gtk_label_get_text(key->label);
}

void ff_key_set_text(FFKey *key, const gchar *text) {
  g_return_if_fail(FF_IS_KEY(key));

  if (!strcmp(text, ff_key_get_text(key)))
    return;

  gtk_label_set_text(key->label, text);
}

enum { PROP_0, PROP_TEXT, LAST_PROP };

static GParamSpec *props[LAST_PROP] = {
    NULL,
};

static void ff_key_set_property(GObject *object, guint prop_id,
                                const GValue *value, GParamSpec *pspec) {
  FFKey *key = FF_KEY(object);

  switch (prop_id) {
  case PROP_TEXT:
    ff_key_set_text(key, g_value_get_string(value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void ff_key_get_property(GObject *object, guint prop_id, GValue *value,
                                GParamSpec *pspec) {
  FFKey *key = FF_KEY(object);

  switch (prop_id) {
  case PROP_TEXT:
    g_value_set_string(value, ff_key_get_text(key));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void ff_key_class_init(FFKeyClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

  object_class->set_property = ff_key_set_property;
  object_class->get_property = ff_key_get_property;
  object_class->dispose = ff_key_dispose;

  props[PROP_TEXT] =
      g_param_spec_string("text", "Text", "Text of the key", "???",
                          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties(object_class, LAST_PROP, props);

  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
  gtk_widget_class_set_template_from_resource(
      widget_class, "/org/ccextractor/FastFingers/ui/key.ui");
  gtk_widget_class_set_css_name(widget_class, "key");
  gtk_widget_class_bind_template_child(widget_class, FFKey, label);
}

static void ff_key_init(FFKey *self) {
  gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget *ff_key_new(const gchar *text) {
  FFKey *_key = g_object_new(FF_TYPE_KEY, "text", text, NULL);

  return GTK_WIDGET(_key);
}
