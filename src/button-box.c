#include "button-box.h"

struct _FFButtonbox
{
  GtkBox parent;

  GtkButton *back;
  GtkButton *settings;
};

G_DEFINE_TYPE (FFButtonbox, ff_buttonbox, GTK_TYPE_BOX);

static void
ff_buttonbox_dispose (GObject *object)
{
  FFButtonbox *self = FF_BUTTONBOX (object);

  G_OBJECT_CLASS (ff_buttonbox_parent_class)->dispose (object);
}

const gchar *
ff_buttonbox_get_backbutton_label (FFButtonbox *buttonbox)
{
  g_return_val_if_fail (FF_IS_BUTTONBOX (buttonbox), NULL);

  return gtk_button_get_label (buttonbox->back);
}

void
ff_buttonbox_set_backbutton_label (FFButtonbox *buttonbox,
				   const gchar *label)
{
  g_return_if_fail (FF_IS_BUTTONBOX (buttonbox));
  
  gtk_button_set_label (buttonbox->back, label);
}

gboolean
ff_buttonbox_get_backbutton_visible (FFButtonbox *buttonbox)
{
  g_return_val_if_fail (FF_IS_BUTTONBOX (buttonbox), -1);

  return gtk_widget_get_visible (GTK_WIDGET (buttonbox->back));
}

void
ff_buttonbox_set_backbutton_visible (FFButtonbox *buttonbox,
				     gboolean visible)
{
  g_return_if_fail (FF_IS_BUTTONBOX (buttonbox));

  gtk_widget_set_visible (GTK_WIDGET (buttonbox->back), visible);
}

gboolean
ff_buttonbox_get_settings_visible (FFButtonbox *buttonbox)
{
  g_return_val_if_fail (FF_IS_BUTTONBOX (buttonbox), -1);

  return gtk_widget_get_visible (GTK_WIDGET (buttonbox->settings));
}

void
ff_buttonbox_set_settings_visible (FFButtonbox *buttonbox,
				   gboolean visible)
{
  g_return_if_fail (FF_IS_BUTTONBOX (buttonbox));

  gtk_widget_set_visible (GTK_WIDGET (buttonbox->settings), visible);
}

enum
  {
    PROP_0,
    PROP_BACK_LABEL,
    PROP_BACK_VISIBLE,
    PROP_SETTINGS_VISIBLE,
    LAST_PROP
  };

static GParamSpec *props[LAST_PROP] = {NULL, };

static void
ff_buttonbox_set_property (GObject *object,
			   guint prop_id,
			   const GValue *value,
			   GParamSpec *pspec)
{
  FFButtonbox *buttonbox = FF_BUTTONBOX (object);

  switch (prop_id)
    {
    case PROP_BACK_LABEL:
      ff_buttonbox_set_backbutton_label (buttonbox, g_value_get_string (value));
      break;
    case PROP_BACK_VISIBLE:
      ff_buttonbox_set_backbutton_visible (buttonbox, g_value_get_boolean (value));
      break;
    case PROP_SETTINGS_VISIBLE:
      ff_buttonbox_set_settings_visible (buttonbox, g_value_get_boolean (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
ff_buttonbox_get_property (GObject *object,
			   guint prop_id,
			   GValue *value,
			   GParamSpec *pspec)
{
  FFButtonbox *buttonbox = FF_BUTTONBOX (object);

  switch (prop_id)
    {
    case PROP_BACK_LABEL:
      ff_buttonbox_get_backbutton_label (buttonbox);
      break;
    case PROP_BACK_VISIBLE:
      ff_buttonbox_get_backbutton_visible (buttonbox);
      break;
    case PROP_SETTINGS_VISIBLE:
      ff_buttonbox_get_settings_visible (buttonbox);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}
             
static void
ff_buttonbox_class_init (FFButtonboxClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->set_property = ff_buttonbox_set_property;
  object_class->get_property = ff_buttonbox_get_property;
  object_class->dispose = ff_buttonbox_dispose;

  props[PROP_BACK_LABEL] =
    g_param_spec_string ("back-label",
			 "Back-Label",
			 "Label of the back button",
			 "Back",
			 G_PARAM_READWRITE|G_PARAM_STATIC_STRINGS);
  
  props[PROP_BACK_VISIBLE] =
    g_param_spec_boolean ("back-visible",
			  "Back-Visible",
			  "Visibility of the back button",
			  0,
			  G_PARAM_READWRITE|G_PARAM_STATIC_STRINGS);

  props[PROP_SETTINGS_VISIBLE] =
    g_param_spec_boolean ("settings-visible",
			  "Settings-Visible",
			  "Visibility of the settings button",
			  1,
			  G_PARAM_READWRITE|G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, LAST_PROP, props);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/ccextractor/FastFingers/ui/button-box.ui");
  gtk_widget_class_set_css_name (widget_class, "buttonbox");
  gtk_widget_class_bind_template_child (widget_class, FFButtonbox, back);
  gtk_widget_class_bind_template_child (widget_class, FFButtonbox, settings);
}

void
back_click_cb (GtkWidget *widget,
		   GdkEvent  *event,
		   gpointer   user_data)
{
  ff_switch_previous ();
}

void
settings_click_cb (GtkWidget *widget,
		   GdkEvent  *event,
		   gpointer   user_data)
{
  ff_switch_page ("Settings");
}


static void
ff_buttonbox_init (FFButtonbox *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  g_signal_connect (GTK_BUTTON (self->back), "clicked", G_CALLBACK (back_click_cb), NULL);
  g_signal_connect (GTK_BUTTON (self->settings), "clicked", G_CALLBACK (settings_click_cb), NULL);
}

GtkWidget*
ff_buttonbox_new (void)
{
  GtkWidget *_buttonbox = g_object_new (FF_TYPE_BUTTONBOX,
					NULL);
  return _buttonbox;
}
