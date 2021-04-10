#include "practice.h"
#include "cards.h"
#include "fflib.h"

int size;
GtkWidget *key_label_arr[3];
int keys[3] = { 0xffe1, 0xffe3, 0x041};
int turn;

GtkWidget *
key_add(GtkBox *parent_box,
	const char *key_description,  const char *key_letter)
{
  GtkWidget *key_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_style_context_add_class ( gtk_widget_get_style_context (key_box), "key_box");
  gtk_box_append (parent_box, key_box);

  GtkWidget *key_label = gtk_label_new(key_letter);
  key_label_arr[size++] = key_label;
  gtk_style_context_add_class ( gtk_widget_get_style_context (key_label), "key_label");
  gtk_widget_set_size_request(key_label, 75, 75);

  gtk_box_append (GTK_BOX(key_box), key_label);

  if (key_description)
    {
      GtkWidget *key_desc = gtk_label_new(key_description);
      gtk_style_context_add_class ( gtk_widget_get_style_context (key_desc), "key_desc");
      gtk_box_append (GTK_BOX(key_box), key_desc);
    }
  return key_box;
}

GtkWidget *
ff_new_practice_page(void)
{
  GtkWidget *practice_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 25);
  gtk_style_context_add_class ( gtk_widget_get_style_context (practice_box), "practice_box");
  gtk_widget_set_valign(practice_box, GTK_ALIGN_CENTER);
  gtk_widget_set_halign(practice_box, GTK_ALIGN_CENTER);
  
  GtkWidget *shortcut_title = gtk_label_new("Select all Artboards");
  gtk_style_context_add_class ( gtk_widget_get_style_context (shortcut_title), "shortcut_title");
  gtk_box_append (GTK_BOX(practice_box), shortcut_title);

  
  GtkWidget *key_list_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 25);
  gtk_style_context_add_class ( gtk_widget_get_style_context (key_list_box), "key_list_box");
  gtk_widget_set_halign(key_list_box, GTK_ALIGN_CENTER);
  gtk_box_append (GTK_BOX(practice_box), key_list_box);

  key_add(GTK_BOX(key_list_box), "SHIFT", "*S");
  key_add(GTK_BOX(key_list_box), "CMD", "*C");
  key_add(GTK_BOX(key_list_box), "", "A");

  return practice_box;
}

gboolean
on_key_press (GtkEventControllerKey *controller,
	      guint keyval,
	      guint keycode,
	      GdkModifierType state,
	      gpointer user_data)
//on_key_press (GtkWidget *widget, GdkEvent *event)
{
  if (page != FF_PRACTICE)
    return 0;

  if (turn == size)
    {
      turn = 0;
      for (int i = 0; i < size; ++i)
	{
	  gtk_style_context_remove_class(gtk_widget_get_style_context(key_label_arr[i]), "key_label_wrong");
	  gtk_style_context_remove_class(gtk_widget_get_style_context(key_label_arr[i]), "key_label_correct");
	  gtk_style_context_add_class ( gtk_widget_get_style_context (key_label_arr[i]), "key_label");
	}

      return 0;
    }
     
  int key = keyval;
  if (keyval >= 0x061 && keyval <= 0x07a)
    {
      key = keyval - 32;
    }

  gtk_style_context_remove_class(gtk_widget_get_style_context(key_label_arr[turn]), "key_label");
  gtk_style_context_add_class(gtk_widget_get_style_context(key_label_arr[turn]),
			      key == keys[turn] ? "key_label_correct" : "key_label_wrong");
  ++turn;
  
  return 0;
}
