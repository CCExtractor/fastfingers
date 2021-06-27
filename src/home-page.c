#include "home-page.h"

void ff_home_page_init(GtkStack *stack)
{
  GtkBuilder *home_builder = gtk_builder_new_from_resource ("/org/ccextractor/FastFingers/ui/home-page.ui");
  GObject *scrolled_window = gtk_builder_get_object (home_builder, "home_scrolled_window");
  GObject *viewport = gtk_builder_get_object (home_builder, "home_viewport");
  GObject *main_box = gtk_builder_get_object (home_builder, "main_box");
  GObject *container = gtk_builder_get_object (home_builder, "home_container");

  FFButtonbox *buttonbox = (FFButtonbox *)ff_buttonbox_new ();
  ff_buttonbox_set_backbutton_visible (buttonbox, FALSE);
  ff_buttonbox_set_settings_visible (buttonbox, TRUE);
  gtk_box_pack_start (GTK_BOX(main_box), GTK_WIDGET (buttonbox), FALSE, FALSE, 0);

  GtkWidget *recent = gtk_label_new("Recent");
  gtk_style_context_add_class (gtk_widget_get_style_context (recent), "home-header");
  gtk_box_pack_start (GTK_BOX(container), recent, FALSE, FALSE, 0);

  GtkWidget *hcont = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_style_context_add_class (gtk_widget_get_style_context (hcont), "home-hcont");
  gtk_box_pack_start (GTK_BOX(container), hcont, FALSE, FALSE, 0);
  
  GtkWidget *lfiller = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_hexpand(lfiller, FALSE);
  gtk_box_pack_start (GTK_BOX(hcont), lfiller, FALSE, FALSE, 0);
    
  GtkWidget *gmail = ff_card_new("Gmail", 0.4);
  gtk_box_pack_start (GTK_BOX(hcont), gmail, FALSE, FALSE, 0);

  GtkWidget *slack = ff_card_new("Slack", 0.23);
  gtk_box_pack_start (GTK_BOX(hcont), slack, FALSE, FALSE, 0);
  
  GtkWidget *vscode = ff_card_new("VS Code", 0.16);
  gtk_box_pack_start (GTK_BOX(hcont), vscode, FALSE, FALSE, 0);

  GtkWidget *sketch = ff_card_new("Sketch", 0.04);
  gtk_box_pack_start (GTK_BOX(hcont), sketch, FALSE, FALSE, 0);
  
  GtkWidget *firefox = ff_card_new("Firefox", 0.04);
  gtk_box_pack_start (GTK_BOX(hcont), firefox, FALSE, FALSE, 0);
  
  GtkWidget *rfiller = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_hexpand(rfiller, FALSE);
  gtk_box_pack_start (GTK_BOX(hcont), rfiller, FALSE, FALSE, 0);

  gtk_stack_add_named(GTK_STACK (stack), GTK_WIDGET (scrolled_window), "Home");
  gtk_widget_show_all (GTK_WIDGET (scrolled_window));
}
