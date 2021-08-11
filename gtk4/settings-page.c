#include "settings-page.h"

void ff_configure_autostart(int state) {
  wordexp_t p;
  wordexp("~/.config/autostart/org.ccextractor.FastFingers.desktop", &p, 0);
  char **w = p.we_wordv;
  if (state) {
    FILE *source = fopen(
        "/usr/share/applications/org.ccextractor.FastFingers.desktop", "r");

    if (!source) {
      fprintf(stderr, "FF-ERROR: Couldn't read desktop file at /usr/share\n");
      return;
    }

    FILE *target = fopen(w[0], "w");

    if (!target) {
      fprintf(
          stderr,
          "FF-ERROR: Couldn't create desktop file at ~/.config/autostart\n");
      fclose(source);
      wordfree(&p);
      return;
    }

    char ch;
    while ((ch = fgetc(source)) != EOF)
      fputc(ch, target);

    fclose(source);
    fclose(target);
  } else {
    int ret = remove(w[0]);
    if (ret)
      fprintf(
          stderr,
          "FF-ERROR: Couldn't remove desktop file at ~/.config/autostart\n");
  }
  wordfree(&p);
}

void ff_check_autostart_state(int state) {
  wordexp_t p;
  wordexp("~/.config/autostart/org.ccextractor.FastFingers.desktop", &p, 0);
  char **w = p.we_wordv;

  int exists = !access(w[0], F_OK);
  if (exists && !state)
    ff_configure_autostart(0);
  else if (!exists && state)
    ff_configure_autostart(1);

  wordfree(&p);
}

void settings_row_activated_cb(GtkListBox *box, GtkListBoxRow *row,
                               gpointer user_data) {
  GSettings *settings = (GSettings *)user_data;
  const char *row_name = gtk_widget_get_name(GTK_WIDGET(row));

  if (!strcmp(row_name, "shortcut")) {
    // To-do: change shortcut
  } else if (!strcmp(row_name, "autostart")) {
    GtkWidget *row_box = gtk_list_box_row_get_child(row);
    GtkWidget *icon = gtk_widget_get_last_child(row_box);

    gboolean new_state = !g_settings_get_boolean(settings, "autostart");
    // To-do: make it really starting auto
    g_settings_set_boolean(settings, "autostart", new_state);
    gtk_label_set_text(GTK_LABEL(icon), new_state ? "Enabled" : "Disabled");
    ff_configure_autostart(new_state);
  } else if (!strcmp(row_name, "reset_progress")) {
    GtkWidget *stack = ff_get_stack();
    ff_reset_progress_page_init(GTK_STACK(stack));
    ff_switch_page("Reset-Progress");
  } else if (!strcmp(row_name, "version")) {
    GtkWidget *window = ff_get_window();
    gtk_show_uri(GTK_WINDOW(window),
                 "https://github.com/CCExtractor/fastfingers/blob/main/NEWS",
                 GDK_CURRENT_TIME);
  } else if (!strcmp(row_name, "license")) {
    GtkWidget *window = ff_get_window();
    gtk_show_uri(GTK_WINDOW(window),
                 "https://github.com/CCExtractor/fastfingers/blob/main/LICENSE",
                 GDK_CURRENT_TIME);
  } else if (!strcmp(row_name, "support")) {
    GtkWidget *window = ff_get_window();
    gtk_show_uri(GTK_WINDOW(window),
                 "https://github.com/CCExtractor/fastfingers/issues",
                 GDK_CURRENT_TIME);
  } else if (!strcmp(row_name, "credits")) {
    ff_switch_page("Credits");
  } else {
  }
}

void ff_settings_page_init(GtkStack *stack) {
  GSettings *settings = g_settings_new("org.ccextractor.FastFingers");

  GtkBuilder *settings_page_builder = gtk_builder_new_from_resource(
      "/org/ccextractor/FastFingers/ui/settings-page.ui");

  GObject *main_box = gtk_builder_get_object(settings_page_builder, "main_box");
  GObject *autostart_label =
      gtk_builder_get_object(settings_page_builder, "autostart_label");
  GObject *list = gtk_builder_get_object(settings_page_builder, "list");

  gboolean current_state = g_settings_get_boolean(settings, "autostart");
  gtk_label_set_text(GTK_LABEL(autostart_label),
                     current_state ? "Enabled" : "Disabled");
  ff_check_autostart_state(current_state);

  g_signal_connect(GTK_WIDGET(list), "row-activated",
                   G_CALLBACK(settings_row_activated_cb), settings);

  gtk_stack_add_named(stack, GTK_WIDGET(main_box), "Settings");
}
