#include "credits-page.h"

void ff_credits_page_init(GtkStack *stack) {
  GtkBuilder *credits_builder = gtk_builder_new_from_resource(
      "/org/ccextractor/FastFingers/ui/credits-page.ui");
  GObject *scrolled_window =
      gtk_builder_get_object(credits_builder, "credits_scrolled_window");

  gtk_stack_add_named(GTK_STACK(stack), GTK_WIDGET(scrolled_window), "Credits");
}
