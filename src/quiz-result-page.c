#include "quiz-result-page.h"

struct {
    cJSON *app;
    dynamicArray *arr;
} global_data;

static void
retryButton_clicked_cb(GtkButton *button,
                       gpointer data) {
    GtkWidget *stack = ff_get_stack();
    ff_quiz_page_init(GTK_STACK(stack), global_data.app, global_data.arr);
    ff_switch_page("quiz");
}

void ff_quiz_result_page_init(cJSON *app, GHashTable *hashTable) {
    GtkStack *stack = (GtkStack *) ff_get_stack();
    GtkWidget *temp = gtk_stack_get_child_by_name(stack, "quiz-result");
    if (temp)
        gtk_container_remove(GTK_CONTAINER(stack), temp);

    GtkBuilder *quiz_result_page_builder = gtk_builder_new_from_resource(
            "/org/ccextractor/FastFingers/ui/quiz-result-page.ui");

    GObject *scrolled_window = gtk_builder_get_object(
            quiz_result_page_builder, "quiz_result_scrolled_window");
    GObject *image = gtk_builder_get_object(quiz_result_page_builder, "image");
    GObject *score_label =
            gtk_builder_get_object(quiz_result_page_builder, "score_label");
    GObject *shortcut_listbox =
            gtk_builder_get_object(quiz_result_page_builder, "shortcut_listbox");
    GObject *retry_button =
            gtk_builder_get_object(quiz_result_page_builder, "retry_button");

    set_scaled_image(GTK_IMAGE(image), cJSON_GetObjectItemCaseSensitive(app, "title")->valuestring, 100);

    dynamicArray *failedShortcutArr = ff_dynamicArray_new(sizeof(cJSON *));
    if (!failedShortcutArr)
        return;

    global_data.app = app;
    global_data.arr = failedShortcutArr;

    int score = 0;
    GList *keys = g_hash_table_get_keys(hashTable);
    for (int i = 0; i < g_list_length(keys); ++i) {
        int *key = g_list_nth_data(keys, i);
        int value = *(int *) g_hash_table_lookup(hashTable, key);

        cJSON *shortcut = ff_find_shortcut_by_id(app, *key);

        if (value)
            ++score;
        else
            ff_dynamicArray_append(failedShortcutArr, &shortcut);

        GtkWidget *row = ff_shortcut_list_row_new(cJSON_GetObjectItemCaseSensitive(shortcut, "title")->valuestring,
                                                  value ? "Correct" : "Wrong");
        gtk_container_add(GTK_CONTAINER(shortcut_listbox), row);
    }

    char *score_str = g_strdup_printf("Score: %d/%d", score, g_list_length(keys));
    gtk_label_set_label(GTK_LABEL(score_label), score_str);
    g_free(score_str);

    g_signal_connect(GTK_WIDGET(retry_button), "clicked", G_CALLBACK(retryButton_clicked_cb), failedShortcutArr);

    gtk_stack_add_named(stack, GTK_WIDGET(scrolled_window), "quiz-result");
    gtk_widget_show_all(GTK_WIDGET(scrolled_window));

    if (score == g_list_length(keys))
        gtk_widget_set_visible(GTK_WIDGET(retry_button), 0);
}
