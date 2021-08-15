#include "quiz-page.h"

static struct {
    int size;
    int idx;
    int category_idx;
    int shortcut_idx;
    int success;
    int idle;
    int *key_arr;
    int question_idx;
    int timer_counter;

    char **str_arr;

    char *app_title;

    cJSON *app;

    GHashTable *hash_table;

    GtkWidget *box;
    GtkWidget *correct_box;

    GtkLabel *question_counter;
    GtkLabel *shortcut_description;
    GtkLabel *timer_label;
    GtkLabel *correct_label;
} glob_data;

static void updateTimerLabel(void) {
    if (glob_data.timer_counter < 0) {
        gtk_label_set_label(glob_data.timer_label, "Time is up!");
    } else {
        char *tmp = g_strdup_printf("%d", glob_data.timer_counter);
        if (!tmp) {
            ff_error("Couldn't allocate memory for timer label!\n");
            return;
        }
        gtk_label_set_label(glob_data.timer_label, tmp);
        g_free(tmp);
    }
}

static void reset_globdata(void) {
    glob_data.size = 0;
    glob_data.idx = 0;
    glob_data.success = 1;
    glob_data.idle = 0;
    glob_data.key_arr = NULL;
    glob_data.str_arr = NULL;
    glob_data.timer_counter = 10;

    remove_style_class(GTK_WIDGET(glob_data.timer_label), "green");
    add_style_class(GTK_WIDGET(glob_data.timer_label), "green");
    remove_style_class(GTK_WIDGET(glob_data.timer_label), "orange");
    remove_style_class(GTK_WIDGET(glob_data.timer_label), "red");

    updateTimerLabel();
}

static gboolean next_quiz_page(gpointer user_data);

static gboolean timer_controller(gpointer user_data) {
    if (glob_data.question_idx > 20)
        return 0;
    if (!g_str_equal("quiz", gtk_stack_get_visible_child_name(GTK_STACK(ff_get_stack()))))
        return 0;
    if (glob_data.idle)
        return 1;

    --glob_data.timer_counter;
    updateTimerLabel();
    if (glob_data.timer_counter < 0) {
        glob_data.success = 0;
        //reveal all keys here
        for (int i = 0; i < glob_data.size; ++i) {
            FFKey *key = (FFKey *) ff_box_nth_child(glob_data.box, i);
            ff_key_set_text(FF_KEY(key), get_keyval_name(glob_data.key_arr[i]));
            ff_key_set_visible(FF_KEY(key), 1);
        }

        glob_data.idle = 1;
        g_timeout_add_seconds(3, G_SOURCE_FUNC(next_quiz_page), NULL);
        return 1;
    } else {
        switch (glob_data.timer_counter) {
            case 6:
                add_style_class(GTK_WIDGET(glob_data.timer_label), "orange");
                break;
            case 3:
                remove_style_class(GTK_WIDGET(glob_data.timer_label), "orange");
                add_style_class(GTK_WIDGET(glob_data.timer_label), "red");
        }

        return 1;
    }
}

static void init_next_shortcut(void) {
    reset_globdata();

    cJSON *group = cJSON_GetObjectItem(glob_data.app, "group");

    cJSON *category = NULL;
    cJSON *shortcut = NULL;
    for (int i = glob_data.category_idx; i < cJSON_GetArraySize(group); ++i, glob_data.shortcut_idx = -1) {
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
        ff_error("Couldn't find shortcut or category!\n");
        return;
    }

    gtk_label_set_text(
            glob_data.shortcut_description,
            cJSON_GetObjectItemCaseSensitive(shortcut, "title")->valuestring);

    char *question_info = g_strdup_printf("Question %d/20", glob_data.question_idx);
    gtk_label_set_text(
            glob_data.question_counter,
            question_info);
    g_free(question_info);


    cJSON *keys = cJSON_GetObjectItemCaseSensitive(shortcut, "keys");
    // If there are more than one keystroke for one shortcut
    if (cJSON_IsArray(cJSON_GetArrayItem(keys, 0)))
        // Select the first one (just temporary)
        keys = cJSON_GetArrayItem(keys, 0);

    glob_data.size = cJSON_GetArraySize(keys);

    glob_data.key_arr = malloc(sizeof(int) * glob_data.size);
    if (!glob_data.key_arr) {
        ff_error("Couldn't match the JSON and category!\n");
        return;
    }

    glob_data.str_arr = malloc(sizeof(char *) * glob_data.size);
    if (!glob_data.key_arr) {
        ff_error("Couldn't allocate space for key_container->str_arr!\n");
        free(glob_data.key_arr);
        return;
    }

    for (int i = 0; i < glob_data.size; ++i) {
        guint keyval = get_keyval_from_name(cJSON_GetArrayItem(keys, i)->valuestring);

        if (keyval == GDK_KEY_VoidSymbol) {
            ff_error("Couldn't get keyval from name %s!\n",
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
    gtk_widget_set_visible(glob_data.correct_box, 0);
    gtk_widget_set_visible(GTK_WIDGET(glob_data.correct_label), 0);
}

static gboolean next_quiz_page(gpointer user_data) {
    int *success = malloc(sizeof(int));
    *success = glob_data.success;
    g_hash_table_insert(glob_data.hash_table,
                        g_strdup(gtk_label_get_text(glob_data.shortcut_description)),
                        success);

    glob_data.idle = 0;
    if (glob_data.question_idx < 1) {
        init_next_shortcut();
        ++glob_data.question_idx;
    } else {
        ff_quiz_result_page_init(glob_data.app_title, glob_data.hash_table);
        ff_switch_page("quiz-result");
    }

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
        if (glob_data.success) {
            g_timeout_add_seconds(1, G_SOURCE_FUNC(next_quiz_page), NULL);
        } else {
            ff_container_remove_all(glob_data.correct_box);
            for (int i = 0; i < glob_data.size; ++i) {
                GtkWidget *correctKey = ff_key_new(glob_data.str_arr[i], 1);
                gtk_box_pack_start(GTK_BOX(glob_data.correct_box), correctKey, FALSE, TRUE, 0);
            }
            gtk_widget_show_all(glob_data.correct_box);
            gtk_widget_show(GTK_WIDGET(glob_data.correct_label));
            g_timeout_add_seconds(3, G_SOURCE_FUNC(next_quiz_page), NULL);
        }
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
    GObject *question_counter = gtk_builder_get_object(quiz_page_builder, "question_counter");
    GObject *timer_label = gtk_builder_get_object(quiz_page_builder, "timer_label");
    GObject *correct_label = gtk_builder_get_object(quiz_page_builder, "correct_label");
    GObject *correct_box = gtk_builder_get_object(quiz_page_builder, "correct_box");

    char *title = g_strdup(cJSON_GetObjectItem(app, "title")->valuestring);
    char button_label[64];
    sprintf(button_label, "< %s", title);

    ff_buttonbox_set_backbutton_label(FF_BUTTONBOX(buttonbox), button_label);

    set_scaled_image(GTK_IMAGE(image), title, 100);
    g_free(title);

    GHashTable *hash_table = g_hash_table_new(g_str_hash, g_str_equal);


    add_style_class(GTK_WIDGET(timer_label), "green");

    glob_data.app = app;
    glob_data.category_idx = 0;
    glob_data.shortcut_idx = -1;
    glob_data.box = GTK_WIDGET(key_box);
    glob_data.correct_box = GTK_WIDGET(correct_box);
    glob_data.correct_label = GTK_LABEL(correct_label);
    glob_data.shortcut_description = GTK_LABEL(shortcut_description);
    glob_data.question_counter = GTK_LABEL(question_counter);
    glob_data.timer_label = GTK_LABEL(timer_label);
    glob_data.question_idx = 1;
    glob_data.hash_table = hash_table;
    glob_data.app_title = cJSON_GetObjectItem(app, "title")->valuestring;

    init_next_shortcut();

    g_signal_connect(G_OBJECT(event_box), "key_press_event",
                     G_CALLBACK(key_press_event_cb), NULL);

    gtk_stack_add_named(GTK_STACK(stack), GTK_WIDGET(event_box), "quiz");
    gtk_widget_show_all(GTK_WIDGET(event_box));
    gtk_widget_grab_focus(GTK_WIDGET(event_box));


    gtk_widget_set_visible(GTK_WIDGET(glob_data.correct_box), 0);
    gtk_widget_set_visible(GTK_WIDGET(glob_data.correct_label), 0);

    g_timeout_add_seconds(1, G_SOURCE_FUNC(timer_controller), NULL);
}