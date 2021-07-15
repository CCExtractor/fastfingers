#include "home-page.h"

void ff_home_page_init(GtkStack *stack) {
  GtkWidget *temp = gtk_stack_get_child_by_name(stack, "home");
  if (temp)
    gtk_container_remove(GTK_CONTAINER(stack), temp);
  GtkBuilder *home_builder = gtk_builder_new_from_resource(
      "/org/ccextractor/FastFingers/ui/home-page.ui");
  GObject *scrolled_window =
      gtk_builder_get_object(home_builder, "home_scrolled_window");
  GObject *container = gtk_builder_get_object(home_builder, "home_container");

  GtkWidget *recent = gtk_label_new("Recent");
  gtk_style_context_add_class(gtk_widget_get_style_context(recent),
                              "home-header");
  gtk_box_pack_start(GTK_BOX(container), recent, FALSE, FALSE, 0);

  GtkWidget *hcont = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_style_context_add_class(gtk_widget_get_style_context(hcont),
                              "home-hcont");
  gtk_widget_set_halign(hcont, GTK_ALIGN_CENTER);
  gtk_box_pack_start(GTK_BOX(container), hcont, FALSE, FALSE, 0);

  GtkWidget *firefox = ff_card_new("Firefox", 0.04);
  gtk_box_pack_start(GTK_BOX(hcont), firefox, FALSE, FALSE, 0);

  gtk_stack_add_named(GTK_STACK(stack), GTK_WIDGET(scrolled_window), "home");
  gtk_widget_show_all(GTK_WIDGET(scrolled_window));
}
