#include "quiz-page.h"

static struct {
    int size;
    int idx;
    int category_idx;
    int shortcut_idx;
    int success;
    int idle;
    int *key_arr;

    char **str_arr;

    cJSON *app;

    GtkWidget *box;
    GtkLabel *shortcut_description;
} glob_data;

static void init_next_shortcut(void) {
    glob_data.size = 0;
    glob_data.idx = 0;
    glob_data.success = 1;
    glob_data.idle = 0;
    glob_data.key_arr = NULL;
    glob_data.str_arr = NULL;

    cJSON *group = cJSON_GetObjectItem(glob_data.app, "group");

    cJSON *category = NULL;
    cJSON *shortcut = NULL;
    for (int i = glob_data.category_idx; i < cJSON_GetArraySize(group); ++i) {
        cJSON *tmp_category = cJSON_GetArrayItem(group, i);
        cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(tmp_category, "shortcuts");
        for (int j = glob_data.shortcut_idx + 1; j < cJSON_GetArraySize(shortcuts); ++j) {
            cJSON *tmp_shortcut = cJSON_GetArrayItem(shortcuts, j);
            if (cJSON_GetObjectItemCaseSensitive(tmp_shortcut, "learned")->valueint == 1) {
                category = tmp_category;
                shortcut = tmp_shortcut;
                glob_data.category_idx = i;
                glob_data.shortcut_idx = j;
                goto end;
            }
        }
    }
    end:

    if (!category || !shortcut) {
        fprintf(stderr,
                "FF-ERROR: Couldn't find shortcut or category!\n");
        return;
    }

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
        GtkWidget *key = ff_key_new(glob_data.str_arr[i], 0);
        gtk_box_pack_start(GTK_BOX(glob_data.box), key, FALSE, TRUE, 0);
    }
    gtk_widget_show_all(glob_data.box);
}

static gboolean next_quiz_page(gpointer user_data) {
    if (glob_data.success) {
        //
    } else {
        //
    }

    init_next_shortcut();
    glob_data.idle = 0;

    return 0;
}

static gboolean
key_press_event_cb(
        GtkWidget *self,
        GdkEventKey *event,
        gpointer user_data
        ) {
    if (glob_data.idle)
        return 0;
    guint keyval;
    gdk_event_get_keyval((const GdkEvent *) event, &keyval);
    GtkWidget *key;
    key = ff_box_nth_child(glob_data.box, glob_data.idx);
    if (key_compare(glob_data.key_arr[glob_data.idx], keyval)) {
        ff_key_set_style(key, "success");
    } else {
        glob_data.success = 0;
        ff_key_set_style(key, "fail");
    }

    ff_key_set_text(FF_KEY(key), get_keyval_name(keyval));
    ff_key_set_visible(FF_KEY(key), 1);

    if (glob_data.size - 1 != glob_data.idx)
        ++glob_data.idx;
    else {
        glob_data.idle = 1;
        g_timeout_add(1000, G_SOURCE_FUNC(next_quiz_page), NULL);
    }
    return 0;
}

void ff_quiz_page_init(GtkStack *stack, cJSON *app) {
    GtkWidget *temp = gtk_stack_get_child_by_name(stack, "quiz");
    if (temp)
        gtk_container_remove(GTK_CONTAINER(stack), temp);

    GtkBuilder *quiz_page_builder = gtk_builder_new_from_resource(
            "/org/ccextractor/FastFingers/ui/quiz-page.ui");

    GObject *event_box =
            gtk_builder_get_object(quiz_page_builder, "event_box");
    GObject *image = gtk_builder_get_object(quiz_page_builder, "image");
    GObject *buttonbox =
            gtk_builder_get_object(quiz_page_builder, "buttonbox");
    GObject *key_box = gtk_builder_get_object(quiz_page_builder, "key_box");
    GObject *shortcut_description =
            gtk_builder_get_object(quiz_page_builder, "shortcut_description");

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
    glob_data.box = GTK_WIDGET(key_box);
    glob_data.shortcut_description = GTK_LABEL(shortcut_description);
    glob_data.category_idx = 0;
    glob_data.shortcut_idx = -1;

    init_next_shortcut();

    g_signal_connect(G_OBJECT(event_box), "key_press_event",
                     G_CALLBACK(key_press_event_cb), NULL);

    gtk_stack_add_named(GTK_STACK(stack), GTK_WIDGET(event_box), "quiz");
    gtk_widget_show_all(GTK_WIDGET(event_box));

}