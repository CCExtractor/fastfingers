#include "home-page.h"

static struct {
  GSequence *categories;
  GSequence *hbox_arr;
} glob_data;

void destroy_cb(gpointer data) { g_free(data); }

void ff_home_page_init(GtkStack *stack) {
  GtkWidget *temp = gtk_stack_get_child_by_name(stack, "home");
  if (temp)
    gtk_container_remove(GTK_CONTAINER(stack), temp);
  GtkBuilder *home_builder = gtk_builder_new_from_resource(
      "/org/ccextractor/FastFingers/ui/home-page.ui");
  GObject *scrolled_window =
      gtk_builder_get_object(home_builder, "home_scrolled_window");
  GObject *container = gtk_builder_get_object(home_builder, "home_container");

  cJSON *recent_json = ff_get_application("appdata");
  cJSON *arr = cJSON_GetObjectItem(recent_json, "recent");
  if (cJSON_GetArraySize(arr) > 0) {
    GtkWidget *recent_title = gtk_label_new("Recent");
    add_style_class(recent_title, "home-header");
    gtk_box_pack_start(GTK_BOX(container), recent_title, FALSE, FALSE, 0);

    GtkWidget *recent_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_style_context_add_class(gtk_widget_get_style_context(recent_container),
                                "home-hcont");
    gtk_widget_set_halign(recent_container, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(container), recent_container, FALSE, FALSE, 0);
    for (int i = 0; i < cJSON_GetArraySize(arr); ++i) {
      GtkWidget *card = ff_card_new("Firefox", 0.04);
      gtk_box_pack_start(GTK_BOX(recent_container), card, FALSE, FALSE, 0);
    }
  }

  glob_data.categories = g_sequence_new(destroy_cb);
  glob_data.hbox_arr = g_sequence_new(NULL);

  DIR *d;
  struct dirent *dir;
  d = opendir("/usr/share/fastfingers/applications");
  if (d) {
    while ((dir = readdir(d))) {
      if (!g_str_has_suffix(dir->d_name, ".json"))
        continue;
      if (!strcmp(dir->d_name, "appdata.json"))
        continue;
      char *name = g_strndup(dir->d_name, strlen(dir->d_name) - 5);

      cJSON *app = ff_get_application(name);

      char *category = cJSON_GetObjectItem(app, "category")->valuestring;
      char *app_title = cJSON_GetObjectItem(app, "title")->valuestring;

      int category_idx = -1;
      for (int i = 0; i < g_sequence_get_length(glob_data.categories); ++i) {
        if (!strcmp(category, g_sequence_get(g_sequence_get_iter_at_pos(
                                  glob_data.categories, i)))) {
          category_idx = i;
          break;
        }
      }

      if (category_idx == -1) {
        GtkWidget *category_label = gtk_label_new(category);
        add_style_class(category_label, "home-header");
        gtk_box_pack_start(GTK_BOX(container), category_label, FALSE, FALSE, 0);

        GtkWidget *category_container =
            gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        gtk_style_context_add_class(
            gtk_widget_get_style_context(category_container), "home-hcont");
        gtk_widget_set_halign(category_container, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(container), category_container, FALSE, FALSE,
                           0);

        g_sequence_append(glob_data.categories, category);
        g_sequence_append(glob_data.hbox_arr, category_container);

        category_idx = g_sequence_get_length(glob_data.categories) - 1;
      }

      double progress = 0;
      int total = 0, learned = 0;
      cJSON *group = cJSON_GetObjectItem(app, "group");
      for (int i = 0; i < cJSON_GetArraySize(group); ++i) {
        cJSON *shortcut_category = cJSON_GetArrayItem(group, i);
        cJSON *shortcuts = cJSON_GetObjectItem(shortcut_category, "shortcuts");
        for (int j = 0; j < cJSON_GetArraySize(shortcuts); ++j) {
          ++total;
          if (cJSON_GetObjectItem(cJSON_GetArrayItem(shortcuts, j), "learned")
                  ->valueint)
            ++learned;
        }
      }

      progress = total ? learned / total : 0;

      GtkWidget *card = ff_card_new(app_title, progress);
      GtkBox *box = g_sequence_get(
          g_sequence_get_iter_at_pos(glob_data.hbox_arr, category_idx));
      gtk_box_pack_start(box, card, FALSE, FALSE, 0);

      g_free(name);
      cJSON_Delete(app);
    }
    closedir(d);
  }

  gtk_stack_add_named(GTK_STACK(stack), GTK_WIDGET(scrolled_window), "home");
  gtk_widget_show_all(GTK_WIDGET(scrolled_window));
}
