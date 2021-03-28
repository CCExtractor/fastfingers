#include "practice.h"
#include "cards.h"

GtkWidget *key_add(GtkContainer *parent_box, const char *key_name,  const char *key_letter)
{
  GtkWidget *key_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_style_context_add_class ( gtk_widget_get_style_context (key_box), "key_box");
  gtk_container_add (parent_box, key_box);

  GtkWidget *key_letter_label = gtk_label_new(key_letter);
  gtk_style_context_add_class ( gtk_widget_get_style_context (key_letter_label), "key_letter_label");
  gtk_widget_set_size_request(key_letter_label, 75, 75);

  gtk_container_add (GTK_CONTAINER(key_box), key_letter_label);

  GtkWidget *key_label = gtk_label_new(key_name);
  gtk_style_context_add_class ( gtk_widget_get_style_context (key_label), "key_label");
  gtk_container_add (GTK_CONTAINER(key_box), key_label);
  
  return key_box;
}

GtkWidget *ff_new_practice_page(void)
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

  GtkWidget *key1 = key_add(GTK_CONTAINER(key_list_box), "SHIFT", "S");
  GtkWidget *key2 = key_add(GTK_CONTAINER(key_list_box), "CMD", "Cs");
  GtkWidget *key3 = key_add(GTK_CONTAINER(key_list_box), "", "A");

  return practice_box;
}

