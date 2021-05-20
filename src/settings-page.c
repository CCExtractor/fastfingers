#include "settings-page.h"

GSettings *settings;

void
settings_row_activated_cb (GtkListBox    *box,
			   GtkListBoxRow *row,
			   gpointer       user_data)
{
  const char *row_name = gtk_widget_get_name ( GTK_WIDGET (row));

  if (!strcmp (row_name, "shortcut"))
    {
      // To-do: change shortcut
    }
  else if (!strcmp (row_name, "autostart"))
    {
      GtkWidget *row_box = gtk_list_box_row_get_child (row);
      GtkWidget *icon = gtk_widget_get_last_child (row_box);

      gboolean new_state = !g_settings_get_boolean (settings, "autostart");
      // To-do: make it really starting auto
      g_settings_set_boolean (settings, "autostart", new_state);
      gtk_label_set_text (GTK_LABEL (icon), new_state ? "Enabled" : "Disabled");
	
    }
  else if (!strcmp (row_name, "reset_progress"))
    {
      // To-do: make a reset progress pop-up screen
      // To-do: reset progress
    }
  else if (!strcmp (row_name, "version"))
    {
      // To-do: open github link at browser
    }
  else if (!strcmp (row_name, "license"))
    {
      // To-do: open github link at browser
    }
  else if (!strcmp (row_name, "support"))
    {
      // To-do: open github link at browser
    }
  else if (!strcmp (row_name, "credits"))
    {
      // To-do 
    }
  else
    {
    }
  
}

void ff_settings_page_init(GtkStack *stack)
{
  settings = g_settings_new ("org.ccextractor.FastFingers");
  
  GtkBuilder *settings_page_builder = gtk_builder_new_from_resource ("/org/ccextractor/FastFingers/ui/settings-page.ui");
  
  GObject *main_box = gtk_builder_get_object (settings_page_builder, "main_box");

  GObject *autostart_label = gtk_builder_get_object (settings_page_builder, "autostart_label");
  
  gboolean current_state = g_settings_get_boolean (settings, "autostart");
  gtk_label_set_text (GTK_LABEL (autostart_label), current_state ? "Y" : "N");
  
  gtk_stack_add_named(stack, GTK_WIDGET (main_box), "Settings");
}
