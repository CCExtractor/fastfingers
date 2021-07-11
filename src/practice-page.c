#include "practice-page.h"

struct {
  int size;
  int idx;
  int category_idx;
  int shortcut_idx;
  int is_test;
  int success;
  int idle;
  int *key_arr;

  char **str_arr;
  const char *row_title;

  cJSON *app;

  GtkWidget *box;
  GtkStack *stack;
  GtkLabel *shortcut_description;
} glob_data;

void init_next_shortcut(void) {
  glob_data.size = 0;
  glob_data.idx = 0;
  glob_data.category_idx = -1;
  glob_data.shortcut_idx = 0;
  glob_data.is_test = 0;
  glob_data.success = 1;
  glob_data.idle = 0;
  glob_data.key_arr = NULL;
  glob_data.str_arr = NULL;

  cJSON *group = cJSON_GetObjectItem(glob_data.app, "group");

  cJSON *category = NULL;

  for (int i = 0; i < cJSON_GetArraySize(group); ++i) {
    cJSON *tmp = cJSON_GetArrayItem(group, i);
    cJSON *title = cJSON_GetObjectItemCaseSensitive(tmp, "title");
    if (!strcmp(title->valuestring, glob_data.row_title)) {
      category = tmp;
      glob_data.category_idx = i;
      break;
    }
  }

  if (!category) {
    fprintf(stderr, "FF-ERROR: Couldn't match the JSON and category!\n");
    return;
  }

  cJSON *shortcut = NULL;
  cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
  for (int i = 0; i < cJSON_GetArraySize(shortcuts); ++i) {
    if (cJSON_GetObjectItemCaseSensitive(cJSON_GetArrayItem(shortcuts, i),
                                         "learned")
            ->valueint == 0) {
      shortcut = cJSON_GetArrayItem(shortcuts, i);
      glob_data.shortcut_idx = i;
      break;
    }
  }

  if (!shortcut)
    shortcut = cJSON_GetArrayItem(shortcuts,
                                  glob_data.shortcut_idx =
                                      rand() % cJSON_GetArraySize(shortcuts));

  gtk_label_set_text(
      glob_data.shortcut_description,
      cJSON_GetObjectItemCaseSensitive(shortcut, "title")->valuestring);

  cJSON *keys = cJSON_GetObjectItemCaseSensitive(shortcut, "keys");
  // If there are more than one key strokes for one shortcut
  if (cJSON_IsArray(cJSON_GetArrayItem(keys, 0)))
    // Select the first one (just temporary)
    keys = cJSON_GetArrayItem(keys, 0);

  glob_data.size = cJSON_GetArraySize(keys);

  glob_data.key_arr = malloc(sizeof(int) * glob_data.size);
  if (!glob_data.key_arr) {
    fprintf(stderr, "FF-ERROR: Couldn't match the JSON and category!\n");
    return;
  }

  glob_data.str_arr = malloc(sizeof(char *) * glob_data.size);
  if (!glob_data.key_arr) {
    fprintf(stderr,
            "FF-ERROR: Couldn't allocate space for key_container->str_arr!\n");
    free(glob_data.key_arr);
    return;
  }

  for (int i = 0; i < glob_data.size; ++i) {
    int keyval = get_keyval_from_name(cJSON_GetArrayItem(keys, i)->valuestring);

    if (keyval == GDK_KEY_VoidSymbol) {
      fprintf(stderr, "FF-ERROR: Couldn't get keyval from name %s!\n",
              cJSON_GetArrayItem(keys, i)->valuestring);
      free(glob_data.key_arr);
      for (int j = 0; j < i; ++j)
        free(glob_data.str_arr[j]);
      free(glob_data.str_arr);
      return;
    }

    glob_data.key_arr[i] = keyval;
    glob_data.str_arr[i] =
        normalize_keyval_name(cJSON_GetArrayItem(keys, i)->valuestring);
  }

  ff_container_remove_all(glob_data.box);
  for (int i = 0; i < glob_data.size; ++i) {
    GtkWidget *key = ff_key_new(glob_data.str_arr[i], 1);
    gtk_box_pack_start(GTK_BOX(glob_data.box), key, FALSE, TRUE, 0);
  }
  gtk_widget_show_all(glob_data.box);
}

void shortcut_learned(void) {
  const char *title = cJSON_GetObjectItem(glob_data.app, "title")->valuestring;
  cJSON *group = cJSON_GetObjectItem(glob_data.app, "group");
  cJSON *category = cJSON_GetArrayItem(group, glob_data.category_idx);
  cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
  cJSON *shortcut = cJSON_GetArrayItem(shortcuts, glob_data.shortcut_idx);
  cJSON *learned = cJSON_GetObjectItemCaseSensitive(shortcut, "learned");
  cJSON_SetIntValue(learned, 1);

  wordexp_t p;
  char **w;
  char file_path[64];

  char *name = ff_simplify_title(title);
  sprintf(file_path, "~/.fastfingers/applications/%s.json", name);
  free(name);

  wordexp(file_path, &p, 0);
  w = p.we_wordv;

  char *out = cJSON_Print(glob_data.app);

  FILE *fp = NULL;
  fp = fopen(w[0], "w");

  if (!fp) {
    fprintf(stderr, "FF-ERROR: Couldn't open file %s: %s\n", file_path,
            strerror(errno));
    goto end;
  }

  int written = fprintf(fp, "%s", out);

  if (written < 0) {
    fprintf(stderr, "FF-ERROR: Couldn't write to file %s: %s\n", file_path,
            strerror(errno));
    goto end;
  }

end:
  if (fp)
    fclose(fp);
  free(out);

  init_next_shortcut();
}

gboolean next_practice_page(gpointer user_data) {
  if (glob_data.success) {
    if (glob_data.is_test)
      shortcut_learned();
    else {
      for (int i = 0; i < glob_data.size; ++i) {
        GtkWidget *key = ff_box_nth_child(glob_data.box, i);
        ff_key_set_style(key, "test");
        ff_key_set_visible(FF_KEY(key), 0);
      }

      glob_data.is_test = 1;
      glob_data.idx = 0;
    }
  } else {
    for (int i = 0; i < glob_data.size; ++i) {
      GtkWidget *key = ff_box_nth_child(glob_data.box, i);
      ff_key_set_style(key, "tooltip");
      ff_key_set_text(FF_KEY(key), normalize_keyval_name(glob_data.str_arr[i]));
      ff_key_set_visible(FF_KEY(key), 1);
    }

    glob_data.is_test = 0;
    glob_data.idx = 0;
    glob_data.success = 1;
  }

  glob_data.idle = 0;

  return 0;
}

gboolean key_pressed_cb(GtkEventControllerKey *controller, guint keyval,
                        guint keycode, GdkModifierType state,
                        gpointer user_data) {
  if (glob_data.idle)
    return 0;

  GtkWidget *key;
  key = ff_box_nth_child(glob_data.box, glob_data.idx);
  if (key_compare(glob_data.key_arr[glob_data.idx], keyval)) {
    ff_key_set_style(key, "success");
  } else {
    glob_data.success = 0;
    ff_key_set_style(key, "fail");
  }

  ff_key_set_text(FF_KEY(key), normalize_keyval_name(gdk_keyval_name(keyval)));
  ff_key_set_visible(FF_KEY(key), 1);

  if (glob_data.size - 1 != glob_data.idx)
    ++glob_data.idx;
  else {
    glob_data.idle = 1;
    g_timeout_add(1000, G_SOURCE_FUNC(next_practice_page), NULL);
  }
  return 0;
}

void ff_practice_page_init(GtkStack *stack, cJSON *app, const char *category) {
  GtkWidget *temp = gtk_stack_get_child_by_name(stack, "practice-page");
  if (temp)
    gtk_container_remove(GTK_CONTAINER(stack), temp);

  GtkBuilder *practice_page_builder = gtk_builder_new_from_resource(
      "/org/ccextractor/FastFingers/ui/practice-page.ui");

  GObject *event_box =
      gtk_builder_get_object(practice_page_builder, "event_box");
  GObject *image = gtk_builder_get_object(practice_page_builder, "image");
  GObject *buttonbox =
      gtk_builder_get_object(practice_page_builder, "buttonbox");
  GObject *key_box = gtk_builder_get_object(practice_page_builder, "key_box");
  GObject *shortcut_description =
      gtk_builder_get_object(practice_page_builder, "shortcut_description");

  char *title = g_strdup(cJSON_GetObjectItem(app, "title")->valuestring);
  char button_label[64];
  sprintf(button_label, "< %s", title);

  ff_buttonbox_set_backbutton_label(FF_BUTTONBOX(buttonbox), button_label);

  char *logo_path = ff_logo_path_gen(title);

  if (logo_path) {
    gtk_image_set_from_resource(GTK_IMAGE(image), logo_path);
    free(logo_path);
  }

  g_free(title);

  glob_data.app = app;
  glob_data.row_title = category;
  glob_data.box = GTK_WIDGET(key_box);
  glob_data.stack = stack;
  glob_data.shortcut_description = GTK_LABEL(shortcut_description);

  init_next_shortcut();

  GtkEventController *key_controller =
      gtk_event_controller_key_new(GTK_WIDGET(event_box));

  g_signal_connect(G_OBJECT(key_controller), "key_pressed",
                   G_CALLBACK(key_pressed_cb), NULL);

  gtk_stack_add_named(GTK_STACK(stack), GTK_WIDGET(event_box), "practice-page");
  gtk_widget_show_all(GTK_WIDGET(event_box));
}
