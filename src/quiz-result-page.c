#include "quiz-result-page.h"

void ff_quiz_result_page_init(const char *app_title, GHashTable *hashTable) {
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

    set_scaled_image(GTK_IMAGE(image), title, 100);

    int score = 0;
    GList *keys = g_hash_table_get_keys(hashTable);
    for (int i = 0; i < g_list_length(keys); ++i){
        char *key = g_list_nth_data(keys, i);
        int value = *(int *)g_hash_table_lookup(hashTable, key);
        if (value)
            ++score;
        GtkWidget *row = ff_shortcut_list_row_new(key,value ? "Correct" : "Wrong");
        gtk_container_add(GTK_CONTAINER(shortcut_listbox), row);
    }

    char *score_str = g_strdup_printf("Score: %d/10", score);
    gtk_label_set_label(GTK_LABEL(score_label), score_str);
    g_free(score_str);

    gtk_stack_add_named(stack, GTK_WIDGET(scrolled_window), "quiz-result");
    gtk_widget_show_all(GTK_WIDGET(scrolled_window));
}
