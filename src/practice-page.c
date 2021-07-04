#include "practice-page.h"

int size = 3;
GtkWidget *key_arr[3];
int keys[3] = {0xffe1, 0xffe3, 0x041};
int turn;

typedef struct key_container {
  size_t size;
  int *key_arr;
  char *str_arr;
} key_container;

void ff_sort_shortcuts(cJSON *app) {
  cJSON *group = cJSON_GetObjectItem(app, "group");

  for (int i = 0; i < cJSON_GetArraySize(group); ++i) {
    cJSON *category = cJSON_GetArrayItem(group, i);
    cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
    for (int i = 0; i < cJSON_GetArraySize(shortcuts) - 1; ++i) {
      for (int j = 0; j < cJSON_GetArraySize(shortcuts) - i - 1; ++j) {
        cJSON *sc_l = cJSON_GetArrayItem(shortcuts, j);
        cJSON *sc_r = cJSON_GetArrayItem(shortcuts, j + 1);
        if (cJSON_GetObjectItemCaseSensitive(sc_l, "learned")->valueint >
            cJSON_GetObjectItemCaseSensitive(sc_r, "learned")->valueint) {
          cJSON *tmp = sc_l;
          cJSON_ReplaceItemViaPointer(shortcuts, sc_l, sc_r);
          cJSON_ReplaceItemViaPointer(shortcuts, sc_r, tmp);
        }
      }
    }
  }
}

key_container *ff_get_key_container(cJSON *app, const char *row_title) {
  key_container *kc = malloc(sizeof(key_container));

  if (!kc) {
    fprintf(stderr, "FF-ERROR: Couldn't allocate memory for key container!\n");
    return NULL;
  }

  cJSON *group = cJSON_GetObjectItem(app, "group");
  cJSON *category = NULL;

  for (int i = 0; i < cJSON_GetArraySize(group); ++i) {
    cJSON *tmp = cJSON_GetArrayItem(group, i);
    cJSON *title = cJSON_GetObjectItemCaseSensitive(category, "title");
    if (!strcmp(title->valuestring, row_title))
      category = tmp;
  }

  if (!category) {
    fprintf(stderr, "FF-ERROR: Couldn't match the JSON and category!\n");
    return NULL;
  }

  cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(category, "shortcuts");

  ff_sort_shortcuts(app);

  cJSON *shortcut = cJSON_GetArrayItem(shortcuts, 0);

  // If all shortcuts are learned, return a random one
  if (cJSON_GetObjectItemCaseSensitive(shortcut, "learned")->valueint == 1)
    shortcut =
        cJSON_GetArrayItem(shortcuts, rand() % cJSON_GetArraySize(shortcuts));
  // else: continue using the first shortcut

  cJSON *keys = cJSON_GetObjectItemCaseSensitive(shortcut, "keys");
  // If there are more than one key strokes for one shortcut
  if (cJSON_IsArray(cJSON_GetArrayItem(keys, 0)))
    // Select the first one (just temporary)
    keys = cJSON_GetArrayItem(keys, 0);

  kc->size = cJSON_GetArraySize(keys);

  ////////

  return kc;
}

void ff_practice_page_init(GtkStack *stack, cJSON *app, const char *category) {
  GtkWidget *temp = gtk_stack_get_child_by_name(stack, "practice-page");
  if (temp)
    gtk_container_remove(GTK_CONTAINER(stack), temp);

  GtkBuilder *practice_page_builder = gtk_builder_new_from_resource(
      "/org/ccextractor/FastFingers/ui/practice-page.ui");

  GObject *main_box = gtk_builder_get_object(practice_page_builder, "main_box");
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

  free(title);

  GtkWidget *lfiller = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_hexpand(lfiller, TRUE);
  gtk_box_pack_start(GTK_BOX(key_box), lfiller, TRUE, TRUE, 0);

  GtkWidget *key1 = key_arr[0] = ff_key_new("SHIFT");
  GtkWidget *key2 = key_arr[1] = ff_key_new("CTRL");
  GtkWidget *key3 = key_arr[2] = ff_key_new("A");
  gtk_box_pack_start(GTK_BOX(key_box), key1, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(key_box), key2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(key_box), key3, TRUE, TRUE, 0);

  for (int i = 0; i < 3; ++i)
    gtk_style_context_add_class(gtk_widget_get_style_context(key_arr[i]),
                                "test");

  GtkWidget *rfiller = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_hexpand(rfiller, TRUE);
  gtk_box_pack_start(GTK_BOX(key_box), rfiller, TRUE, TRUE, 0);

  gtk_stack_add_named(GTK_STACK(stack), GTK_WIDGET(main_box), "practice-page");
  gtk_widget_show_all(GTK_WIDGET(main_box));
}

gboolean key_pressed_cb(GtkEventControllerKey *controller, guint keyval,
                        guint keycode, GdkModifierType state,
                        gpointer user_data) {
  if (strcmp(ff_get_current_page(), "practice-page"))
    return 0;

  fprintf(stderr, "0x%04x\n", keyval);
  if (turn == size) {
    turn = 0;
    for (int i = 0; i < size; ++i) {
      gtk_style_context_remove_class(gtk_widget_get_style_context(key_arr[i]),
                                     "wrong");
      gtk_style_context_remove_class(gtk_widget_get_style_context(key_arr[i]),
                                     "correct");
      gtk_style_context_add_class(gtk_widget_get_style_context(key_arr[i]),
                                  "test");
    }
    return 0;
  }

  int key = keyval;
  if (keyval >= 0x061 && keyval <= 0x07a) {
    key = keyval - 32;
  }

  gtk_style_context_remove_class(gtk_widget_get_style_context(key_arr[turn]),
                                 "test");
  gtk_style_context_add_class(gtk_widget_get_style_context(key_arr[turn]),
                              key == keys[turn] ? "correct" : "wrong");
  ++turn;

  return 0;
}
