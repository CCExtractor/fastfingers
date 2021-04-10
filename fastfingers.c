#include "fflib.h"
#include "home.h"
#include "cards.h"
#include "practice.h"

GtkWidget *stack;
GtkWidget *window;

int page = FF_HOME;
    
static void
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

int
main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("com.aliereny.fastfingers", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
