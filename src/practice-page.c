#include "practice-page.h"

typedef struct key_container {
  size_t size;
  int *key_arr;
  char **str_arr;
  int idx;
  int category_idx;
  int shortcut_idx;
  int is_test;
  int success;
  cJSON *app;
  const char *row_title;
} key_container;

typedef struct page_data {
  GtkWidget *box;
  key_container *kc;
  GtkStack *stack;
} page_data;

page_data *new_page_data(GtkWidget *box, key_container *kc, GtkStack *stack) {
  page_data *pd = malloc(sizeof(page_data));
  if (!pd) {
    fprintf(stderr, "FF-ERROR: Couldn't allocate memory for page data!\n");
    return NULL;
  }

  pd->box = box;
  pd->kc = kc;
  pd->stack = stack;

  return pd;
}

void ff_free_key_container(void *ptr) {
  key_container *kc = (key_container *)ptr;
  free(kc->key_arr);
  for (int i = 0; i < kc->size; ++i)
    free(kc->str_arr[i]);
  free(kc->str_arr);
  free(kc);
  fprintf(stderr, "freed\n");
}

int get_keyval_from_name(const char *str) {
  if (!strcmp("Control", str))
    return gdk_keyval_from_name("Control_L");
  if (!strcmp("Alt", str))
    return gdk_keyval_from_name("Alt_L");
  if (!strcmp("Shift", str))
    return gdk_keyval_from_name("Shift_L");
  return gdk_keyval_from_name(str);
}

key_container *ff_get_key_container(cJSON *app, const char *row_title) {
  key_container *kc = malloc(sizeof(key_container));

  if (!kc) {
    fprintf(stderr, "FF-ERROR: Couldn't allocate memory for key container!\n");
    return NULL;
  }

  kc->size = 0;
  kc->key_arr = NULL;
  kc->str_arr = NULL;
  kc->idx = kc->is_test = 0;
  kc->success = 1;
  kc->shortcut_idx = 0;
  kc->app = app;
  kc->row_title = row_title;

  cJSON *group = cJSON_GetObjectItem(app, "group");

  cJSON *category = NULL;

  for (int i = 0; i < cJSON_GetArraySize(group); ++i) {
    cJSON *tmp = cJSON_GetArrayItem(group, i);
    cJSON *title = cJSON_GetObjectItemCaseSensitive(tmp, "title");
    if (!strcmp(title->valuestring, row_title)) {
      category = tmp;
      kc->category_idx = i;
      break;
    }
  }

  if (!category) {
    fprintf(stderr, "FF-ERROR: Couldn't match the JSON and category!\n");
    return NULL;
  }

  cJSON *shortcut = NULL;
  cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
  for (int i = 0; i < cJSON_GetArraySize(shortcuts); ++i) {
    if (cJSON_GetObjectItemCaseSensitive(cJSON_GetArrayItem(shortcuts, i),
                                         "learned")
            ->valueint == 0) {
      shortcut = cJSON_GetArrayItem(shortcuts, i);
      kc->shortcut_idx = i;
      break;
    }
  }

  if (!shortcut)
    shortcut = cJSON_GetArrayItem(
        shortcuts, kc->shortcut_idx = rand() % cJSON_GetArraySize(shortcuts));

  cJSON *keys = cJSON_GetObjectItemCaseSensitive(shortcut, "keys");
  // If there are more than one key strokes for one shortcut
  if (cJSON_IsArray(cJSON_GetArrayItem(keys, 0)))
    // Select the first one (just temporary)
    keys = cJSON_GetArrayItem(keys, 0);

  kc->size = cJSON_GetArraySize(keys);

  kc->key_arr = malloc(sizeof(int) * kc->size);
  if (!kc->key_arr) {
    fprintf(stderr, "FF-ERROR: Couldn't match the JSON and category!\n");
    free(kc);
    return NULL;
  }

  kc->str_arr = malloc(sizeof(char *) * kc->size);
  if (!kc->key_arr) {
    fprintf(stderr,
            "FF-ERROR: Couldn't allocate space for key_container->str_arr!\n");
    free(kc->key_arr);
    free(kc);
    return NULL;
  }

  for (int i = 0; i < kc->size; ++i) {
    int keyval = get_keyval_from_name(cJSON_GetArrayItem(keys, i)->valuestring);

    if (keyval == GDK_KEY_VoidSymbol) {
      fprintf(stderr, "FF-ERROR: Couldn't get keyval from name %s!\n",
              cJSON_GetArrayItem(keys, i)->valuestring);
      free(kc->key_arr);
      for (int j = 0; j < i; ++j)
        free(kc->str_arr[j]);
      free(kc->str_arr);
      free(kc);
      return NULL;
    }

    kc->key_arr[i] = keyval;
    kc->str_arr[i] =
        normalize_keyval_name(cJSON_GetArrayItem(keys, i)->valuestring);
  }

  return kc;
}

gboolean key_pressed_cb(GtkEventControllerKey *controller, guint keyval,
                        guint keycode, GdkModifierType state, page_data *pd) {
  static key_container *kc = NULL;
  if (kc != pd->kc)
    kc = pd->kc;
  if (!kc)
    return 1;

  GtkWidget *key;
  key = ff_box_nth_child(pd->box, kc->idx);
  if (kc->key_arr[kc->idx] == keyval) {
    ff_key_set_style(key, "success");
  } else {
    kc->success = 0;
    ff_key_set_style(key, "fail");
  }

  ff_key_set_text(FF_KEY(key), normalize_keyval_name(gdk_keyval_name(keyval)));
  ff_key_set_visible(FF_KEY(key), 1);

  if (kc->size - 1 != kc->idx) {
    ++kc->idx;
  } else {
    clock_t t = clock();
    while ((clock() - t) / CLOCKS_PER_SEC < 1)
      ; // null statement

    if (kc->success) {
      if (kc->is_test) {
        const char *title = cJSON_GetObjectItem(kc->app, "title")->valuestring;
        cJSON *group = cJSON_GetObjectItem(kc->app, "group");
        cJSON *category = cJSON_GetArrayItem(group, kc->category_idx);
        cJSON *shortcuts =
            cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
        cJSON *shortcut = cJSON_GetArrayItem(shortcuts, kc->shortcut_idx);
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

        char *out = cJSON_Print(kc->app);

        FILE *fp = NULL;
        fp = fopen(w[0], "w");

        if (!fp) {
          fprintf(stderr, "FF-ERROR: Couldn't open file %s: %s\n", file_path,
                  strerror(errno));
          goto end;
        }

        int written = fprintf(fp, "%s", out);

        if (written < 0) {
          fprintf(stderr, "FF-ERROR: Couldn't write to file %s: %s\n",
                  file_path, strerror(errno));
          goto end;
        }

      end:
        if (fp)
          fclose(fp);
        free(out);

        ff_practice_page_init(pd->stack, kc->app, kc->row_title);
      } else {
        for (int i = 0; i < kc->size; ++i) {
          key = ff_box_nth_child(pd->box, i);
          ff_key_set_style(key, "test");
        }

        kc->is_test = 1;
        kc->idx = 0;
      }
    } else {
      ff_practice_page_init(pd->stack, kc->app, kc->row_title);
    }
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

  key_container *kc = ff_get_key_container(app, category);
  for (int i = 0; i < kc->size; ++i) {
    GtkWidget *key = ff_key_new(kc->str_arr[i], 1);
    gtk_box_pack_start(GTK_BOX(key_box), key, FALSE, TRUE, 0);
  }

  page_data *pd = new_page_data(GTK_WIDGET(key_box), kc, stack);
  GtkEventController *key_controller =
      gtk_event_controller_key_new(GTK_WIDGET(event_box));
  g_signal_connect(G_OBJECT(key_controller), "key_pressed",
                   G_CALLBACK(key_pressed_cb), pd);

  gtk_stack_add_named(GTK_STACK(stack), GTK_WIDGET(event_box), "practice-page");
  gtk_widget_show_all(GTK_WIDGET(event_box));
}
