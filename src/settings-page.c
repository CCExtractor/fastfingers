#include "settings-page.h"

void
settings_row_activated_cb (GtkListBox    *box,
			   GtkListBoxRow *row,
			   gpointer       user_data)
{
  gtk_list_box_unselect_row (box, row);
  const char *row_name = gtk_widget_get_name ( GTK_WIDGET (row));

  printf ("%s\n", row_name);
  GtkWidget *row_box = gtk_list_box_row_get_child (row);
  
}

void ff_settings_page_init(GtkStack *stack)
{
  GtkBuilder *settings_page_builder = gtk_builder_new_from_resource ("/org/ccextractor/FastFingers/ui/settings-page.ui");
  
  GObject *main_box = gtk_builder_get_object (settings_page_builder, "main_box");

  gtk_stack_add_named(stack, GTK_WIDGET (main_box), "Settings");
}
