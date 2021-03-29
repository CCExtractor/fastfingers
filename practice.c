#include "practice.h"
#include "cards.h"

int size;
GtkWidget *key_label_arr[3];
int keys[3] = { 0xffe1, 0xffe3, 0x041};
int turn;

GtkWidget *
key_add(GtkContainer *parent_box,
	const char *key_description,  const char *key_letter)
{
  GtkWidget *key_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_style_context_add_class ( gtk_widget_get_style_context (key_box), "key_box");
  gtk_container_add (parent_box, key_box);

  GtkWidget *key_label = gtk_label_new(key_letter);
  key_label_arr[size++] = key_label;
  gtk_style_context_add_class ( gtk_widget_get_style_context (key_label), "key_label");
  gtk_widget_set_size_request(key_label, 75, 75);

  gtk_container_add (GTK_CONTAINER(key_box), key_label);

  if (key_description)
    {
      GtkWidget *key_desc = gtk_label_new(key_description);
      gtk_style_context_add_class ( gtk_widget_get_style_context (key_desc), "key_desc");
      gtk_container_add (GTK_CONTAINER(key_box), key_desc);
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
  gtk_container_add (GTK_CONTAINER(practice_box), shortcut_title);

  
  GtkWidget *key_list_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 25);
  gtk_style_context_add_class ( gtk_widget_get_style_context (key_list_box), "key_list_box");
  gtk_widget_set_halign(key_list_box, GTK_ALIGN_CENTER);
  gtk_container_add (GTK_CONTAINER(practice_box), key_list_box);

  key_add(GTK_CONTAINER(key_list_box), "SHIFT", "*S");
  key_add(GTK_CONTAINER(key_list_box), "CMD", "*C");
  key_add(GTK_CONTAINER(key_list_box), "", "A");

  return practice_box;
}

gboolean
on_key_press (GtkWidget *widget, GdkEventKey *event)
{
  if (turn == size)
    return -1;
  
  int key = event->keyval;
  if (event->keyval >= 0x061 && event->keyval <= 0x07a)
    {
      key = event->keyval - 32;
    }

  gtk_widget_reset_style(key_label_arr[turn]);
  gtk_style_context_add_class(gtk_widget_get_style_context(key_label_arr[turn]),
			      key == keys[turn] ? "key_label_correct" : "key_label_wrong");
  ++turn;
  return -1;
}
