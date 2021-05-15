#include "practice-page.h"

int size = 3;
GtkWidget *key_arr[3];
int keys[3] = { 0xffe1, 0xffe3, 0x041};
int turn;

void
ff_practice_page_init(GtkStack *stack, const char *title)
{
  GtkWidget *temp;
  if (temp = gtk_stack_get_child_by_name (stack, "practice-page"))
    gtk_stack_remove (stack, temp);
  
  GtkBuilder *practice_page_builder = gtk_builder_new_from_resource("/org/fastfingers/FastFingers/ui/practice-page.ui");

  GObject *main_box = gtk_builder_get_object (practice_page_builder, "main_box");
  GObject *image = gtk_builder_get_object (practice_page_builder, "image");
  GObject *buttonbox = gtk_builder_get_object (practice_page_builder, "buttonbox");
  GObject *back_button = gtk_builder_get_object (practice_page_builder, "back_button");
  GObject *key_box = gtk_builder_get_object (practice_page_builder, "key_box");

  char button_label[64];
  sprintf (button_label, "< %s", title);
  
  ff_buttonbox_set_backbutton_label (FF_BUTTONBOX (buttonbox), button_label);
   
  char *logo_path = ff_logo_path_gen(title);

  if (logo_path)
    {
      gtk_image_set_from_resource(GTK_IMAGE (image), logo_path);
      free(logo_path);
    }

  GtkWidget *lfiller = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_hexpand(lfiller, TRUE);
  gtk_box_append(GTK_BOX(key_box), lfiller);
  
  GtkWidget *key1 = key_arr[0] = ff_key_new ("SHIFT");
  GtkWidget *key2 = key_arr[1] = ff_key_new ("CTRL");
  GtkWidget *key3 = key_arr[2] = ff_key_new ("A");
  gtk_box_append (GTK_BOX (key_box), key1);
  gtk_box_append (GTK_BOX (key_box), key2);
  gtk_box_append (GTK_BOX (key_box), key3);

  for (int i = 0; i < 3; ++i)
    gtk_style_context_add_class (gtk_widget_get_style_context (key_arr[i]), "normal");

  GtkWidget *rfiller = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_hexpand(rfiller, TRUE);
  gtk_box_append(GTK_BOX(key_box), rfiller);

  gtk_stack_add_named(GTK_STACK (stack), GTK_WIDGET (main_box), "practice-page");
}

gboolean
on_key_press (GtkEventControllerKey *controller,
	      guint keyval,
	      guint keycode,
	      GdkModifierType state,
	      gpointer user_data)
{
  if (strcmp(ff_get_current_page (), "practice-page"))
    return 0;

  if (turn == size)
    {
      turn = 0;
      for (int i = 0; i < size; ++i)
	{
	  gtk_style_context_remove_class(gtk_widget_get_style_context(key_arr[i]), "wrong");
	  gtk_style_context_remove_class(gtk_widget_get_style_context(key_arr[i]), "correct");
	  gtk_style_context_add_class (gtk_widget_get_style_context (key_arr[i]), "normal");
	}
      return 0;
    }

  int key = keyval;
  if (keyval >= 0x061 && keyval <= 0x07a)
    {
      key = keyval - 32;
    }

  gtk_style_context_remove_class(gtk_widget_get_style_context(key_arr[turn]), "normal");
  gtk_style_context_add_class(gtk_widget_get_style_context(key_arr[turn]),
			      key == keys[turn] ? "correct" : "wrong");
  ++turn;

  return 0;
}
