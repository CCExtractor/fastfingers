#include "cheatsheet.h"

#include "ff-utils.h"
#include <gtk/gtk.h>

GtkWidget *window;

static void search_changed_cb(GtkSearchEntry *entry, gpointer *userdata) {
  const char *text;
  text = gtk_editable_get_text(GTK_EDITABLE(entry));
  printf("%s\n", text);
}

void cheatscreen_toggle(void) {
  if (gtk_widget_get_visible(window))
    gtk_widget_hide(window);
  else
    gtk_widget_show(window);
}

void cheatscreen_init(void) {
  ff_init_css();

  GtkBuilder *builder = gtk_builder_new_from_resource(
      "/org/ccextractor/FastFingers/ui/cheatsheet.ui");
  window = (GtkWidget *)gtk_builder_get_object(builder, "window");
  gtk_window_set_decorated(GTK_WINDOW(window), TRUE);
  gtk_window_maximize(GTK_WINDOW(window));

  GtkWidget *entry = gtk_search_entry_new();
  gtk_widget_set_halign(entry, GTK_ALIGN_CENTER);

  GtkWidget *searchbar =
      (GtkWidget *)gtk_builder_get_object(builder, "searchbar");
  gtk_search_bar_connect_entry(GTK_SEARCH_BAR(searchbar), GTK_EDITABLE(entry));
  gtk_search_bar_set_show_close_button(GTK_SEARCH_BAR(searchbar), FALSE);
  gtk_search_bar_set_child(GTK_SEARCH_BAR(searchbar), entry);
  gtk_search_bar_set_key_capture_widget(GTK_SEARCH_BAR(searchbar), window);

  gtk_widget_set_size_request(entry, 550, 45);

  g_signal_connect(entry, "search-changed", G_CALLBACK(search_changed_cb),
                   NULL);
}
