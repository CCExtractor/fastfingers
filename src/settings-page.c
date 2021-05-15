#include "settings-page.h"

void ff_settings_page_init(GtkStack *stack)
{
  GtkBuilder *settings_page_builder = gtk_builder_new_from_resource ("/org/fastfingers/FastFingers/ui/settings-page.ui");
  
  GObject *main_box = gtk_builder_get_object (settings_page_builder, "main_box");

  gtk_stack_add_named(stack, GTK_WIDGET (main_box), "Settings");
}
