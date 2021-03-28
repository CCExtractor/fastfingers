#include "home.h"
#include "cards.h"

GtkWidget *ff_new_home_page(void)
{
  GtkWidget *scroll_window, *viewport;
  
  scroll_window = gtk_scrolled_window_new(NULL, NULL);
  viewport = gtk_viewport_new(NULL, NULL);
  gtk_container_add (GTK_CONTAINER (scroll_window), viewport);
  
  GtkWidget *grid = gtk_grid_new();
  gtk_widget_set_size_request(grid, 950, 550);
  gtk_style_context_add_class ( gtk_widget_get_style_context (grid), "grid");
  gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
  gtk_container_add(GTK_CONTAINER (viewport), grid);

  GtkWidget *label_recent = gtk_label_new("Recent");
  gtk_style_context_add_class ( gtk_widget_get_style_context (label_recent), "title");
  gtk_widget_set_halign(label_recent, GTK_ALIGN_START);
  gtk_grid_attach(GTK_GRID(grid), label_recent, 0, 0, 1, 1);

  GtkWidget *card1 = attach_new_card(GTK_GRID(grid), 1, "Sketch", 23, 143, sketch);
  GtkWidget *card2 = attach_new_card(GTK_GRID(grid), 0, "VS Code", 3, 123, vscode);

  GtkWidget *label_development = gtk_label_new("Development");
  gtk_style_context_add_class ( gtk_widget_get_style_context (label_development), "title");
  gtk_widget_set_halign(label_development, GTK_ALIGN_START);
  gtk_grid_attach(GTK_GRID(grid), label_development, 0, 2, 1, 1);

  attach_new_card(GTK_GRID(grid), 3, "Gmail", 26, 220, gmail);
  attach_new_card(GTK_GRID(grid), 0, "Slack", 15, 142, slack);

  return scroll_window;
}
