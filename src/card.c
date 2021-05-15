#include "card.h"

struct _FFCard
{
  GtkBox parent;

  GtkImage *image;
  GtkLabel *label;
  GtkLevelBar *level_bar;
  GtkGesture *gest_click;
};

G_DEFINE_TYPE (FFCard, ff_card, GTK_TYPE_BOX);

static void
ff_card_dispose (GObject *object)
{
  FFCard *self = FF_CARD (object);

  G_OBJECT_CLASS (ff_card_parent_class)->dispose (object);
}

const gchar *
ff_card_get_title (FFCard *card)
{
  g_return_val_if_fail (FF_IS_CARD (card), NULL);

  return gtk_label_get_text(card->label);
}

void
ff_card_set_title (FFCard *card,
		   const gchar* title)
{
  g_return_if_fail (FF_IS_CARD (card));

  if (!strcmp(title, ff_card_get_title(card)))
    return;
  
  gtk_label_set_text(card->label, title);
  
  char *logo_path = ff_logo_path_gen(title);

  if (logo_path)
    {
      gtk_image_set_from_resource(FF_CARD(card)->image, logo_path);
      free(logo_path);
    }
}

double
ff_card_get_progress (FFCard *card)
{
  g_return_val_if_fail (FF_IS_CARD (card), -1);

  return gtk_level_bar_get_value (card->level_bar);
}

void
ff_card_set_progress (FFCard *card,
		      double progress)
{
  g_return_if_fail (FF_IS_CARD (card));

  gtk_level_bar_set_value (card->level_bar, progress);
}

enum
  {
    PROP_0,
    PROP_TITLE,
    PROP_PROGRESS,
    LAST_PROP
  };

static GParamSpec *props[LAST_PROP] = {NULL, };

static void
ff_card_set_property (GObject *object,
		      guint prop_id,
		      const GValue *value,
		      GParamSpec *pspec)
{
  FFCard *card = FF_CARD (object);

  switch (prop_id)
    {
    case PROP_TITLE:
      ff_card_set_title (card, g_value_get_string (value));
      break;
    case PROP_PROGRESS:
      ff_card_set_progress (card, g_value_get_double (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
ff_card_get_property (GObject *object,
		      guint prop_id,
		      GValue *value,
		      GParamSpec *pspec)
{
  FFCard *card = FF_CARD (object);

  switch (prop_id)
    {
    case PROP_TITLE:
      g_value_set_string (value, ff_card_get_title (card));
      break;
    case PROP_PROGRESS:
      g_value_set_double (value, ff_card_get_progress (card));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}
             


static void
ff_card_class_init (FFCardClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->set_property = ff_card_set_property;
  object_class->get_property = ff_card_get_property;
  object_class->dispose = ff_card_dispose;

  props[PROP_TITLE] =
    g_param_spec_string ("title",
			 "Title",
			 "Name of the software as title",
			 "unknown app",
			 G_PARAM_READWRITE|G_PARAM_STATIC_STRINGS);
  
  props[PROP_PROGRESS] =
    g_param_spec_double ("progress",
			 "Progress",
			 "Progress for the software",
			 0,
			 1,
			 0,
			 G_PARAM_READWRITE|G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, LAST_PROP, props);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/fastfingers/FastFingers/ui/card.ui");
  gtk_widget_class_set_css_name (widget_class, "card");
  gtk_widget_class_bind_template_child (widget_class, FFCard, image);
  gtk_widget_class_bind_template_child (widget_class, FFCard, label);
  gtk_widget_class_bind_template_child (widget_class, FFCard, level_bar);
  gtk_widget_class_bind_template_child (widget_class, FFCard, gest_click);
}

static void
ff_card_init (FFCard *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  g_signal_connect (self->gest_click, "pressed", G_CALLBACK (card_click_cb), self);
}

GtkWidget*
ff_card_new (const gchar *title, double progress)
{
  GtkWidget *_card = g_object_new (FF_TYPE_CARD,
				   "title", title,
				   "progress", progress,
				   NULL);
  return _card;
}

static void
card_click_cb (GtkGestureClick *gesture,
	  gint             n_press,
	  gdouble          x,
	  gdouble          y,
	  GtkWidget       *card)
{
  GtkWidget *head = gtk_widget_get_first_child(card);
  GtkWidget *title = gtk_widget_get_next_sibling(head);
  const char *title_str = gtk_label_get_text(GTK_LABEL(title));
  fprintf(stderr, "Debug: App card %s choosed\n", title_str);
  
  ff_application_page_init (GTK_STACK(stack), title_str);
  ff_switch_page (title_str);
}
