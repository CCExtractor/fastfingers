#include "cheatsheet.h"

char *get_active_window(void) {
#ifdef _X11_FOUND
  return get_active_from_xorg();
#else
  return get_active_from_gnome_shell();
#endif
}

int strcmpic(const char *str1, const char *str2) {
  int len = strlen(str1);
  if (len != strlen(str2))
    return 0;

  for (int i = 0; i < len; ++i) {
    if (toupper(str1[i]) != toupper(str2[i]))
      return 0;
  }

  return 1;
}

gboolean button_press_event_cb(GtkStatusIcon *status_icon, GdkEvent *event,
                               GtkWidget *window) {
  if (gtk_widget_get_visible(window))
    return 0;

  gtk_widget_show_all(window);

  return 0;
}

gboolean delete_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data) {
  gtk_window_iconify(GTK_WINDOW(widget));
  return 1;
}

char *normalize_name(const char *name) {
  if (strcmpic(name, "firefox"))
    return "firefox";

  return NULL;
}

GtkWidget *new_label_with_class(const char *text, const char *class) {
  GtkWidget *label = gtk_label_new(text);
  add_style_class(label, class);
  return label;
}

GtkWidget *new_shortcut_box(void) {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_widget_set_margin_left(box, 30);
  gtk_widget_set_margin_right(box, 30);
  gtk_widget_set_hexpand(box, 1);
  return box;
}

GtkWidget *add_shortcut_title(GtkWidget *hbox, const char *shortcut_title) {
  GtkWidget *shortcut_label =
      new_label_with_class(shortcut_title, "shortcut-title");
  gtk_label_set_line_wrap_mode(GTK_LABEL(shortcut_label), PANGO_WRAP_WORD_CHAR);
  gtk_label_set_justify(GTK_LABEL(shortcut_label), GTK_JUSTIFY_FILL);
  gtk_label_set_max_width_chars(GTK_LABEL(shortcut_label), 50);
  gtk_widget_set_halign(shortcut_label, GTK_ALIGN_START);
  gtk_widget_set_hexpand(shortcut_label, 1);
  gtk_label_set_line_wrap(GTK_LABEL(shortcut_label), 1);
  gtk_box_pack_start(GTK_BOX(hbox), shortcut_label, FALSE, TRUE, 0);
  return shortcut_label;
}

gboolean update(GtkWidget *parent) {
  static const char *last_window = NULL;
  char *active_window = get_active_window();

  if (active_window) {
    const char *normalized = normalize_name(active_window);

    if ((!normalized) || (last_window && !strcmp(last_window, normalized)))
      goto out;

    last_window = normalized;

    fprintf(stderr, "%s\n", normalized);

    GList *children = gtk_container_get_children(GTK_CONTAINER(parent));
    GtkWidget *former_box = (GtkWidget *)(g_list_last(children)->data);
    gtk_container_remove(GTK_CONTAINER(parent), former_box);

    GtkWidget *header = (GtkWidget *)(g_list_nth_data(children, 0));
    GList *header_children = gtk_container_get_children(GTK_CONTAINER(header));
    GtkImage *logo = (GtkImage *)(g_list_nth_data(header_children, 0));
    set_scaled_image(logo, normalized, 25);

    GtkLabel *app_label = (GtkLabel *)(g_list_nth_data(header_children, 1));
    char *app_title = g_strdup(normalized);
    app_title[0] = toupper(app_title[0]);
    gtk_label_set_text(app_label, app_title);
    free(app_title);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(parent), vbox, FALSE, TRUE, 0);

    cJSON *app = ff_get_application(normalized);
    cJSON *group = cJSON_GetObjectItem(app, "group");

    for (int i = 0; i < cJSON_GetArraySize(group); ++i) {
      cJSON *category = cJSON_GetArrayItem(group, i);
      cJSON *category_title =
          cJSON_GetObjectItemCaseSensitive(category, "title");
      GtkWidget *category_label =
          new_label_with_class(category_title->valuestring, "category-title");
      gtk_box_pack_start(GTK_BOX(vbox), category_label, FALSE, TRUE, 0);

      cJSON *shortcuts =
          cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
      for (int j = 0; j < cJSON_GetArraySize(shortcuts); ++j) {
        cJSON *shortcut = cJSON_GetArrayItem(shortcuts, j);

        GtkWidget *hbox = new_shortcut_box();
        cJSON *shortcut_title =
            cJSON_GetObjectItemCaseSensitive(shortcut, "title");
        add_shortcut_title(hbox, shortcut_title->valuestring);

        cJSON *keys = cJSON_GetObjectItemCaseSensitive(shortcut, "keys");
        if (cJSON_IsArray(cJSON_GetArrayItem(keys, 0)))
          keys = cJSON_GetArrayItem(keys, 0);

        for (int k = 0; k < cJSON_GetArraySize(keys); ++k) {
          cJSON *key = cJSON_GetArrayItem(keys, k);
          char *key_str = normalize_keyval_name(key->valuestring);
          GtkWidget *key_label = new_label_with_class(key_str, "shortcut-key");

          gtk_widget_set_valign(key_label, GTK_ALIGN_CENTER);
          free(key_str);
          gtk_box_pack_start(GTK_BOX(hbox), key_label, FALSE, TRUE, 0);
        }

        gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
        gtk_widget_show_all(parent);
      }
    }
  out:;
    free(active_window);
  }

  return 1;
}

int strstric(const char *str1, const char *str2) {
  int ret;
  char *src = g_strdup(str1);
  char *phrase = g_strdup(str2);

  for (int i = 0; i < strlen(src); ++i)
    src[i] = tolower(src[i]);
  for (int i = 0; i < strlen(phrase); ++i)
    phrase[i] = tolower(phrase[i]);

  ret = !!strstr(src, phrase);

  g_free(src);
  g_free(phrase);

  return ret;
}

void changed_cb(GtkEditable *editable, GtkWidget *main_box) {
  const char *text = gtk_entry_get_text(GTK_ENTRY(editable));

  GList *main_box_children =
      gtk_container_get_children(GTK_CONTAINER(main_box));
  GtkWidget *vbox = (GtkWidget *)(g_list_nth_data(main_box_children, 2));

  GList *children = gtk_container_get_children(GTK_CONTAINER(vbox));

  int len = g_list_length(children);

  if (!len)
    return;

  GtkWidget *last_title = (GtkWidget *)g_list_nth_data(children, 0);
  int found = 0;

  for (int i = 1; i < len; ++i) {
    GtkWidget *widget = (GtkWidget *)g_list_nth_data(children, i);

    if (GTK_IS_BOX(widget)) {
      GList *hbox_children = gtk_container_get_children(GTK_CONTAINER(widget));
      GtkLabel *label = (GtkLabel *)(g_list_nth_data(hbox_children, 0));
      if (strstric(gtk_label_get_text(label), text)) {
        found = 1;
        gtk_widget_set_visible(widget, 1);
      } else
        gtk_widget_set_visible(widget, 0);
    }

    if (GTK_IS_LABEL(widget) || i == len - 1) {
      if (!found)
        gtk_widget_set_visible(last_title, 0);
      else
        gtk_widget_set_visible(last_title, 1);
      last_title = widget;
      found = 0;
    }
  }
}

static void activate(GtkApplication *app, gpointer user_data) {
  ff_init_css();

  GtkBuilder *builder = gtk_builder_new_from_resource(
      "/org/ccextractor/FastFingers/ui/cheatsheet.ui");

  GtkWidget *window = (GtkWidget *)gtk_builder_get_object(builder, "window");
  GtkWidget *main_box =
      (GtkWidget *)gtk_builder_get_object(builder, "main-box");
  gtk_widget_show_all(window);
  gtk_window_iconify(GTK_WINDOW(window));
  gtk_window_set_title(GTK_WINDOW(window), "FastFingers Cheatsheet");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);

  g_signal_connect(G_OBJECT(window), "delete-event",
                   G_CALLBACK(delete_event_cb), NULL);

  GtkWidget *entry = (GtkWidget *)gtk_builder_get_object(builder, "entry");
  g_signal_connect(GTK_EDITABLE(entry), "changed", G_CALLBACK(changed_cb),
                   main_box);

  GtkStatusIcon *status_icon = gtk_status_icon_new_from_file(
      "/usr/share/icons/hicolor/32x32/apps/fastfingers.png");

  g_signal_connect(status_icon, "button-press-event",
                   G_CALLBACK(button_press_event_cb), window);

  g_timeout_add(500, G_SOURCE_FUNC(update), main_box);

  gtk_main();
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.ccextractor.FastFingers.cheatsheet",
                            G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
