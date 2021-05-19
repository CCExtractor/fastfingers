#include "application-page.h"

const char *_title;

void
application_row_activated_cb (GtkListBox    *box,
			      GtkListBoxRow *row,
			      gpointer       user_data)
{
  gtk_list_box_unselect_row (box, row);
  ff_practice_page_init (GTK_STACK(stack), _title);
  ff_switch_page ("practice-page");
}

void ff_application_page_init(GtkStack *stack, const char *title)
{
  GtkWidget *temp;
  if (temp = gtk_stack_get_child_by_name (stack, title))
    gtk_stack_remove (stack, temp);

  _title = title;
    
  GtkBuilder *application_page_builder = gtk_builder_new_from_resource ("/org/ccextractor/FastFingers/ui/application-page.ui");
  
  GObject *scrolled_window = gtk_builder_get_object (application_page_builder, "application_scrolled_window");
  GObject *main_box = gtk_builder_get_object (application_page_builder, "main_box");
  GObject *image = gtk_builder_get_object (application_page_builder, "image");
  GObject *list_box = gtk_builder_get_object (application_page_builder, "list_box");

  char *logo_path = ff_logo_path_gen(title);

  if (logo_path)
    {
      gtk_image_set_from_resource(GTK_IMAGE (image), logo_path);
      free(logo_path);
    }

  gtk_stack_add_named(stack, GTK_WIDGET (scrolled_window), title);
}
