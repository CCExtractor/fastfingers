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
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect (window, "key_press_event", G_CALLBACK (on_key_press), NULL);
    
  stack = gtk_stack_new();
  gtk_container_add (GTK_CONTAINER (window), stack);  
  
  GtkWidget *home_page = ff_new_home_page();
  gtk_stack_add_named(GTK_STACK(stack), home_page, "home");
  gtk_widget_show(home_page);
  gtk_stack_set_visible_child(GTK_STACK(stack), home_page);

  gtk_widget_show_all(window);                
  gtk_main();
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
