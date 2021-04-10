#include "cards.h"
#include "fflib.h"
#include "practice.h"

static void
on_click (GtkGestureClick *gesture,
	  gint             n_press,
	  gdouble          x,
	  gdouble          y,
	  GtkWidget   *card)
{
  GtkWidget *head = gtk_widget_get_first_child(card);
  GtkWidget *title = gtk_widget_get_next_sibling(head);
  fprintf(stderr, "Debug: App card %s choosed\n", gtk_label_get_text(GTK_LABEL(title)));
  
  GtkWidget *practice_page = ff_new_practice_page();
  gtk_stack_add_named(GTK_STACK(stack), practice_page, "practice");
  gtk_widget_show(practice_page);
  gtk_stack_set_visible_child(GTK_STACK(stack), practice_page);
  gtk_widget_show(window);                

  page = FF_PRACTICE;
}

GtkWidget*
attach_new_card (GtkGrid *grid, int i_top,
		 const char *title,
		 int completed, int total)
{
  char logo_path[64];
  char info[64];
  static int top;
  static int left;

  logo_path_gen(logo_path, title);
  
  if (i_top)
    {
      top  = i_top;
      left = 0;
    }

  GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(card, 450, 300);
  gtk_style_context_add_class (gtk_widget_get_style_context (card), "card");

  GtkWidget *card_logo = gtk_image_new_from_file (logo_path);
  gtk_style_context_add_class (gtk_widget_get_style_context (card_logo), "card_logo");
  gtk_widget_set_size_request(card_logo, 75, 75);
  gtk_widget_set_halign(card_logo, GTK_ALIGN_START);
  gtk_widget_set_vexpand(card_logo, 1);
  gtk_box_append(GTK_BOX(card), card_logo);

  GtkWidget *card_title = gtk_label_new(title);
  gtk_style_context_add_class (gtk_widget_get_style_context (card_title), "card_title");
  gtk_widget_set_halign(card_title, GTK_ALIGN_START);
  gtk_widget_set_valign(card_title, GTK_ALIGN_END);
  gtk_widget_set_vexpand(card_title, 1);
  gtk_box_append(GTK_BOX(card), card_title);

  sprintf(info, "%d/%d mastered", completed, total);
  GtkWidget *card_info = gtk_label_new(info);
  gtk_style_context_add_class (gtk_widget_get_style_context (card_info), "card_info");
  gtk_widget_set_halign(card_info, GTK_ALIGN_START);
  gtk_widget_set_valign(card_info, GTK_ALIGN_END);
  gtk_widget_set_vexpand(card_info, 1);
  gtk_box_append(GTK_BOX(card), card_info);

  GtkWidget *card_lbar = gtk_level_bar_new();
  gtk_widget_set_size_request(card_lbar, -1, 1);
  gtk_style_context_add_class (gtk_widget_get_style_context (card_lbar), "card_lbar");
  gtk_level_bar_set_min_value(GTK_LEVEL_BAR(card_lbar), 0);
  gtk_level_bar_set_max_value(GTK_LEVEL_BAR(card_lbar), total);
  gtk_level_bar_set_value(GTK_LEVEL_BAR(card_lbar), completed);
  
  gtk_widget_set_vexpand(card_lbar, 1);
  gtk_box_append(GTK_BOX(card), card_lbar);

  GtkGesture *click_gesture = gtk_gesture_click_new ();
  g_signal_connect (click_gesture, "pressed", G_CALLBACK (on_click), card);
  gtk_widget_add_controller (card, GTK_EVENT_CONTROLLER (click_gesture));

  gtk_grid_attach(grid, card, left, top, 1, 1);
  
  left = (left + 1) % 2;
  top += 1 - left;

  return card;
}
