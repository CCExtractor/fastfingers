#include "application-page.h"

void
row_activated_cb (GtkListBox    *box,
		  GtkListBoxRow *row,
		  gpointer       title)
{
  gtk_list_box_unselect_row (box, row);
  ff_practice_page_init (GTK_STACK(stack), title);
  ff_switch_page ("practice-page");
}

void ff_application_page_init(GtkStack *stack, const char *title)
{
  GtkBuilder *application_page_builder = gtk_builder_new_from_resource ("/org/fastfingers/FastFingers/ui/application-page.ui");
  
  GObject *scrolled_window = gtk_builder_get_object (application_page_builder, "application_scrolled_window");
  GObject *main_box = gtk_builder_get_object (application_page_builder, "main_box");
  GObject *image = gtk_builder_get_object (application_page_builder, "image");
  GObject *list_box = gtk_builder_get_object (application_page_builder, "list_box");

  g_signal_connect(list_box, "row-activated", G_CALLBACK(row_activated_cb), (gpointer) title);
  
  char *logo_path = ff_logo_path_gen(title);

  if (logo_path)
    {
      gtk_image_set_from_resource(GTK_IMAGE (image), logo_path);
      free(logo_path);
    }

  gtk_stack_add_named(stack, GTK_WIDGET (scrolled_window), title);
}
