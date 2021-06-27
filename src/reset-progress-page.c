#include "reset-progress-page.h"

GtkWidget *message, *info, *button_box;

void ff_reset_progress_page_init(GtkStack *stack)
{
  GtkWidget *temp;
  if (temp = gtk_stack_get_child_by_name (stack, "Reset-Progress"))
    gtk_container_remove (GTK_CONTAINER(stack), temp);
  
  GtkBuilder *reset_progress_builder = gtk_builder_new_from_resource ("/org/ccextractor/FastFingers/ui/reset-progress-page.ui");
  GObject *main_box = gtk_builder_get_object (reset_progress_builder, "main_box");
  message = GTK_WIDGET (gtk_builder_get_object (reset_progress_builder, "message"));
  info = GTK_WIDGET (gtk_builder_get_object (reset_progress_builder, "info"));
  button_box = GTK_WIDGET (gtk_builder_get_object (reset_progress_builder, "button_box"));

  gtk_stack_add_named(GTK_STACK (stack), GTK_WIDGET (main_box), "Reset-Progress");
  gtk_widget_show_all (GTK_WIDGET (main_box));
}

void
no_clicked_cb (GtkButton *button,
		gpointer   user_data)
{
  ff_switch_previous ();
}

void
yes_clicked_cb (GtkButton *button,
		gpointer   user_data)
{
  // Reset progress here
  gtk_label_set_text (GTK_LABEL (message), "Your progress has been reset.");
  gtk_label_set_text (GTK_LABEL (info), "You may continue.");
  gtk_widget_set_visible (button_box, 0);
}

