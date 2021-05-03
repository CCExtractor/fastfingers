#include "ff_application.h"

GtkWidget *stack;
GtkWidget *window;

int page = FFP_HOME;
    
void
activate (GtkApplication *app,
          gpointer        user_data)
{
  ff_init_css();

  window = gtk_application_window_new (app);
  gtk_window_set_title(GTK_WINDOW(window), "FastFingers");
  gtk_window_set_default_size (GTK_WINDOW (window), 1000, 600);
  gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_window_destroy), NULL);

  stack = gtk_stack_new();
  gtk_window_set_child (GTK_WINDOW (window), stack);

  GtkEventController *key_controller = gtk_event_controller_key_new();
  g_signal_connect (key_controller, "key_pressed", G_CALLBACK(on_key_press), NULL);
  gtk_widget_add_controller(stack, key_controller);
  
  GtkWidget *home_page = ff_new_home_page();
  gtk_stack_add_named(GTK_STACK(stack), home_page, "home");
  gtk_widget_show(home_page);
  gtk_stack_set_visible_child(GTK_STACK(stack), home_page);
  
  gtk_widget_show(window);                
}

static void
ff_init_css (void)
{
  GtkCssProvider *provider;
  GdkDisplay *display;

  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  gtk_style_context_add_provider_for_display (display, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  const gchar *myCssFile = "ff_style.css";

  gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile));

  g_object_unref (provider);
}
