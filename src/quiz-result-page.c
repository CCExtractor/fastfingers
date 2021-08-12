#include "quiz-result-page.h"

static void
wrap_hash_map_to_list(
        const char *key,
        const int *value,
        GtkContainer *container) {
    GtkWidget *row = ff_shortcut_list_row_new(key,
                                              *value ? "Correct" : "Wrong");
    gtk_container_add(GTK_CONTAINER(container), row);
}

void ff_quiz_result_page_init(const char *app_title, GHashTable *hashTable) {
    GtkStack *stack = ff_get_stack();
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

    char *logo_path = ff_logo_path_gen(app_title);

    if (logo_path) {
        gtk_image_set_from_resource(GTK_IMAGE(image), logo_path);
        free(logo_path);
    }

    int score = 0;
    GList *vals = g_hash_table_get_values(hashTable);
    for (int i = 0; i < g_list_length(vals); ++i)
        if (*(int *) g_list_nth_data(vals, i))
            ++score;

    char *score_str = g_strdup_printf("Score: %d/10", score);
    gtk_label_set_label(GTK_LABEL(score_label), score_str);
    g_free(score_str);

    g_hash_table_foreach(hashTable, wrap_hash_map_to_list, GTK_CONTAINER(shortcut_listbox));


    gtk_stack_add_named(stack, GTK_WIDGET(scrolled_window), "quiz-result");
    gtk_widget_show_all(GTK_WIDGET(scrolled_window));
}
