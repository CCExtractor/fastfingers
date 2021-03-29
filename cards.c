#include "cards.h"

void handler(GtkWidget *widget, GdkEventButton *event)
{
  GtkWidget *card = gtk_bin_get_child(GTK_BIN(widget));
  GList *children = gtk_container_get_children(GTK_CONTAINER(card));
  GtkWidget *title = g_list_nth_data(children, 1);
  fprintf(stderr, "%s\n", gtk_label_get_text(GTK_LABEL(title)));
}

GtkWidget*
attach_new_card (GtkGrid *grid, int i_top,
		 const char *title,
		 int completed, int total)
{
  char logo_path[64];
  char info[64];
  double fraction = (double) completed / total;
  static int top;
  static int left;

  logo_path_gen(logo_path, title);
  
  if (i_top)
    {
      top  = i_top;
      left = 0;
    }
  
  GtkWidget *event_box = gtk_event_box_new();

  GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(card, 450, 300);
  gtk_style_context_add_class (gtk_widget_get_style_context (card), "card");
  
  GtkWidget *card_logo = gtk_image_new_from_file (logo_path);
  gtk_style_context_add_class (gtk_widget_get_style_context (card_logo), "card_logo");
  gtk_widget_set_halign(card_logo, GTK_ALIGN_START);
  gtk_widget_set_vexpand(card_logo, 1);
  gtk_container_add(GTK_CONTAINER(card), card_logo);
  
  GtkWidget *card_title = gtk_label_new(title);
  gtk_style_context_add_class (gtk_widget_get_style_context (card_title), "card_title");
  gtk_widget_set_halign(card_title, GTK_ALIGN_START);
  gtk_widget_set_valign(card_title, GTK_ALIGN_END);
  gtk_widget_set_vexpand(card_title, 1);
  gtk_container_add(GTK_CONTAINER(card), card_title);

  sprintf(info, "%d/%d mastered", completed, total);
  GtkWidget *card_info = gtk_label_new(info);
  gtk_style_context_add_class (gtk_widget_get_style_context (card_info), "card_info");
  gtk_widget_set_halign(card_info, GTK_ALIGN_START);
  gtk_widget_set_valign(card_info, GTK_ALIGN_END);
  gtk_widget_set_vexpand(card_info, 1);
  gtk_container_add(GTK_CONTAINER(card), card_info);
  
  GtkWidget *card_pbar = gtk_progress_bar_new();
  gtk_style_context_add_class (gtk_widget_get_style_context (card_pbar), "card_pbar");
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(card_pbar), fraction);
  gtk_widget_set_vexpand(card_pbar, 1);
  gtk_container_add(GTK_CONTAINER(card), card_pbar);

  g_signal_connect (event_box, "button_press_event", G_CALLBACK(handler), NULL);
  gtk_container_add(GTK_CONTAINER(event_box), card);
  gtk_grid_attach(grid, event_box, left, top, 1, 1);

  left = (left + 1) % 2;
  top += 1 - left;
  
  return card;
}
