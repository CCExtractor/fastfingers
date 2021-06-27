#include "home-page.h"

void ff_home_page_init(GtkStack *stack)
{
  GtkBuilder *home_builder = gtk_builder_new_from_resource ("/org/ccextractor/FastFingers/ui/home-page.ui");
  GObject *scrolled_window = gtk_builder_get_object (home_builder, "home_scrolled_window");
  GObject *viewport = gtk_builder_get_object (home_builder, "home_viewport");
  GObject *main_box = gtk_builder_get_object (home_builder, "main_box");
  GObject *container = gtk_builder_get_object (home_builder, "home_container");
  GObject *settings_button = gtk_builder_get_object (home_builder, "settings_button");

  GtkWidget *recent = gtk_label_new("Recent");
  gtk_style_context_add_class (gtk_widget_get_style_context (recent), "home-header");
  gtk_box_append(GTK_BOX(container), recent);

  GtkWidget *hcont = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_style_context_add_class (gtk_widget_get_style_context (hcont), "home-hcont");
  gtk_box_append(GTK_BOX(container), hcont);

  GtkWidget *lfiller = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_hexpand(lfiller, TRUE);
  gtk_box_append(GTK_BOX(hcont), lfiller);
    
  GtkWidget *gmail = ff_card_new("Gmail", 0.4);
  gtk_box_append(GTK_BOX(hcont), gmail);

  GtkWidget *slack = ff_card_new("Slack", 0.23);
  gtk_box_append(GTK_BOX(hcont), slack);
  
  GtkWidget *vscode = ff_card_new("VS Code", 0.16);
  gtk_box_append(GTK_BOX(hcont), vscode);

  GtkWidget *sketch = ff_card_new("Sketch", 0.04);
  gtk_box_append(GTK_BOX(hcont), sketch);
  
  GtkWidget *firefox = ff_card_new("Firefox", 0.04);
  gtk_box_append(GTK_BOX(hcont), firefox);
  
  GtkWidget *rfiller = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_hexpand(rfiller, TRUE);
  gtk_box_append(GTK_BOX(hcont), rfiller);
  
  GtkWidget *button_box = ff_buttonbox_new ();
  gtk_box_prepend(GTK_BOX(main_box), button_box);

  gtk_stack_add_named(GTK_STACK (stack), GTK_WIDGET (scrolled_window), "Home");
}
