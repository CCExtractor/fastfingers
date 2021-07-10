#include "key.h"

struct _FFKey {
  GtkLabel parent;
  const char *text;
};

G_DEFINE_TYPE(FFKey, ff_key, GTK_TYPE_LABEL);

static void ff_key_dispose(GObject *object) {
  G_OBJECT_CLASS(ff_key_parent_class)->dispose(object);
  // FFKey *key = FF_KEY(object);
  // g_free(key->text);
}

const gchar *ff_key_get_text(FFKey *key) {
  g_return_val_if_fail(FF_IS_KEY(key), NULL);

  return key->text;
}

void ff_key_set_text(FFKey *key, const gchar *text) {
  g_return_if_fail(FF_IS_KEY(key));
  key->text = g_strdup(text);
}

int ff_key_get_visible(FFKey *key) {
  g_return_val_if_fail(FF_IS_KEY(key), 0);

  return !!strcmp(gtk_label_get_text(GTK_LABEL(key)), "");
}

void ff_key_set_visible(FFKey *key, int state) {
  g_return_if_fail(FF_IS_KEY(key));

  state ? gtk_label_set_text(GTK_LABEL(key), ff_key_get_text(key))
        : gtk_label_set_text(GTK_LABEL(key), "");
}

enum { PROP_0, PROP_TEXT, PROP_IS_VISIBLE, LAST_PROP };

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
  case PROP_IS_VISIBLE:
    ff_key_set_visible(key, g_value_get_int(value));
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
  case PROP_IS_VISIBLE:
    g_value_set_int(value, ff_key_get_visible(key));
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
  props[PROP_IS_VISIBLE] =
      g_param_spec_int("is-visible", "Is Visible", "Visibility of key value", 0,
                       1, 1, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties(object_class, LAST_PROP, props);

  gtk_widget_class_set_template_from_resource(
      widget_class, "/org/ccextractor/FastFingers/ui/key.ui");
  gtk_widget_class_set_css_name(widget_class, "key");
}

static void ff_key_init(FFKey *self) {
  gtk_widget_init_template(GTK_WIDGET(self));
}

void ff_key_set_style(GtkWidget *key, const char *style) {
  GtkStyleContext *context = gtk_widget_get_style_context(key);
  static const char *classes[] = {"tooltip", "test", "success", "fail"};
  for (int i = 0; i < sizeof(classes) / sizeof(classes[0]); ++i)
    if (gtk_style_context_has_class(context, classes[i]))
      gtk_style_context_remove_class(context, classes[i]);

  add_style_class(key, style);
}

GtkWidget *ff_key_new(const gchar *text, int is_visible) {
  FFKey *_key =
      g_object_new(FF_TYPE_KEY, "text", text, "is-visible", is_visible, NULL);
  ff_key_set_style(GTK_WIDGET(_key), "tooltip");
  return GTK_WIDGET(_key);
}
