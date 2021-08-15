#include "card.h"

struct _FFCard {
  GtkEventBox parent;

  GtkBox *box;
  GtkImage *image;
  GtkLabel *label;
  GtkLevelBar *level_bar;

  int size;
};

G_DEFINE_TYPE(FFCard, ff_card, GTK_TYPE_EVENT_BOX);

static void ff_card_dispose(GObject *object) {
  G_OBJECT_CLASS(ff_card_parent_class)->dispose(object);
}

const gchar *ff_card_get_title(FFCard *card) {
  g_return_val_if_fail(FF_IS_CARD(card), NULL);

  return gtk_label_get_text(card->label);
}

void ff_card_set_title(FFCard *card, const gchar *title) {
  g_return_if_fail(FF_IS_CARD(card));

  if (!strcmp(title, ff_card_get_title(card)))
    return;

  gtk_label_set_text(card->label, title);

  set_scaled_image(card->image, title, card->size);
}

double ff_card_get_progress(FFCard *card) {
  g_return_val_if_fail(FF_IS_CARD(card), -1);

  return gtk_level_bar_get_value(card->level_bar);
}

void ff_card_set_progress(FFCard *card, double progress) {
  g_return_if_fail(FF_IS_CARD(card));

  gtk_level_bar_set_value(card->level_bar, progress);
}

double ff_card_get_size(FFCard *card) {
  g_return_val_if_fail(FF_IS_CARD(card), -1);

  return card->size;
}

void ff_card_set_size(FFCard *card, int size) {
  g_return_if_fail(FF_IS_CARD(card));

  card->size = size;
}

enum { PROP_0, PROP_TITLE, PROP_PROGRESS, PROP_SIZE, LAST_PROP };

static GParamSpec *props[LAST_PROP] = {
    NULL,
};

static void ff_card_set_property(GObject *object, guint prop_id,
                                 const GValue *value, GParamSpec *pspec) {
  FFCard *card = FF_CARD(object);

  switch (prop_id) {
  case PROP_TITLE:
    ff_card_set_title(card, g_value_get_string(value));
    break;
  case PROP_PROGRESS:
    ff_card_set_progress(card, g_value_get_double(value));
    break;
  case PROP_SIZE:
    ff_card_set_size(card, g_value_get_int(value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void ff_card_get_property(GObject *object, guint prop_id, GValue *value,
                                 GParamSpec *pspec) {
  FFCard *card = FF_CARD(object);

  switch (prop_id) {
  case PROP_TITLE:
    g_value_set_string(value, ff_card_get_title(card));
    break;
  case PROP_PROGRESS:
    g_value_set_double(value, ff_card_get_progress(card));
    break;
  case PROP_SIZE:
    g_value_set_int(value, ff_card_get_size(card));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void ff_card_class_init(FFCardClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

  object_class->set_property = ff_card_set_property;
  object_class->get_property = ff_card_get_property;
  object_class->dispose = ff_card_dispose;

  props[PROP_TITLE] = g_param_spec_string(
      "title", "Title", "Name of the software as title", "unknown app",
      G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_PROGRESS] =
      g_param_spec_double("progress", "Progress", "Progress for the software",
                          0, 1, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_SIZE] = g_param_spec_int(
      "size", "Size", "Size of one side of the card image in pixels", 0, 150,
      65, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties(object_class, LAST_PROP, props);

  gtk_widget_class_set_template_from_resource(
      widget_class, "/org/ccextractor/FastFingers/ui/card.ui");
  gtk_widget_class_set_css_name(widget_class, "card");
  gtk_widget_class_bind_template_child(widget_class, FFCard, box);
  gtk_widget_class_bind_template_child(widget_class, FFCard, image);
  gtk_widget_class_bind_template_child(widget_class, FFCard, label);
  gtk_widget_class_bind_template_child(widget_class, FFCard, level_bar);
}

static void card_click_cb(FFCard *card, GdkEvent *event, gpointer user_data) {
  GList *children = gtk_container_get_children(GTK_CONTAINER(card->box));
  GtkWidget *title = (GtkWidget *)(g_list_nth(children, 1)->data);
  const char *title_str = gtk_label_get_text(GTK_LABEL(title));

  GtkWidget *stack = ff_get_stack();

  ff_application_page_init(GTK_STACK(stack), title_str);
  ff_switch_page(title_str);
}

static void ff_card_init(FFCard *self) {
  gtk_widget_init_template(GTK_WIDGET(self));
  g_signal_connect(self, "button-press-event", G_CALLBACK(card_click_cb), NULL);
}

GtkWidget *ff_card_new(const gchar *title, double progress) {
  return g_object_new(FF_TYPE_CARD, "title", title, "progress", progress, NULL);
}
