#include "cheatsheet.h"

GtkWidget *window;

static void
search_changed_cb (GtkSearchEntry *entry,
                   gpointer       *userdata)
{
  const char *text;
  text = gtk_entry_get_text (GTK_ENTRY (entry));
  printf ("%s\n", text);
}

gboolean
button_press_event_cb (GtkStatusIcon *status_icon,
               GdkEvent      *event,
               gpointer       user_data)
{
  fprintf(stderr, "pre\n");  
}
  
static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  ff_init_css ();

  GtkBuilder *builder = gtk_builder_new_from_resource ("/org/ccextractor/FastFingers/ui/cheatsheet.ui");
  //  window = gtk_application_window_new (app);
  
  window = (GtkWidget *)gtk_builder_get_object (builder, "window");
  gtk_window_set_title (GTK_WINDOW (window), "FastFingers Cheatsheet");
  gtk_window_set_decorated (GTK_WINDOW (window), FALSE);
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ALWAYS);

  GtkWidget *entry = gtk_search_entry_new ();
  gtk_widget_set_halign (entry, GTK_ALIGN_CENTER);

  /*
  GtkWidget *searchbar = (GtkWidget *)gtk_builder_get_object (builder, "searchbar");
  gtk_search_bar_connect_entry (GTK_SEARCH_BAR (searchbar), GTK_ENTRY (entry));
  gtk_search_bar_set_show_close_button (GTK_SEARCH_BAR (searchbar), FALSE);

  gtk_widget_set_size_request (entry, 550, 45);

  g_signal_connect (entry, "search-changed",
		    G_CALLBACK (search_changed_cb), NULL);

  */
  
  GtkStatusIcon *status_icon = gtk_status_icon_new_from_file ("/usr/share/icons/hicolor/32x32/apps/fastfingers.png");
  gtk_widget_show_all (GTK_WIDGET (window));

  g_signal_connect (status_icon, "button-press-event", G_CALLBACK (button_press_event_cb), NULL);

  gtk_main ();
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.ccextractor.Fastfingers.cheatsheet", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
