#include "application-page.h"

void application_row_activated_cb(GtkListBox *box, GtkListBoxRow *row,
                                  cJSON *app) {
    gtk_list_box_unselect_row(box, row);
    GtkWidget *stack = ff_get_stack();
    ff_practice_page_init(
            GTK_STACK(stack), app,
            ff_shortcut_list_row_get_left_text((FFShortcutListRow *) row));
    ff_switch_page("practice");
}

void
quizButton_clicked_cb(GtkButton *button,
                      cJSON *app) {
    GtkWidget *stack = ff_get_stack();
    ff_quiz_page_init(GTK_STACK(stack), app);
    ff_switch_page("quiz");
}

void ff_application_page_init(GtkStack *stack, const char *title) {
    GtkWidget *temp = gtk_stack_get_child_by_name(stack, title);
    if (temp)
        gtk_container_remove(GTK_CONTAINER(stack), temp);

    GtkBuilder *application_page_builder = gtk_builder_new_from_resource(
            "/org/ccextractor/FastFingers/ui/application-page.ui");

    GObject *scrolled_window = gtk_builder_get_object(
            application_page_builder, "application_scrolled_window");
    GObject *image = gtk_builder_get_object(application_page_builder, "image");
    GObject *list_topic =
            gtk_builder_get_object(application_page_builder, "list_topic");
    GObject *main_box =
            gtk_builder_get_object(application_page_builder, "main_box");
    GObject *recent_list =
            gtk_builder_get_object(application_page_builder, "list_recent");
    GObject *recent_title =
            gtk_builder_get_object(application_page_builder, "recent_title");
    GObject *quiz_button =
            gtk_builder_get_object(application_page_builder, "quiz_button");

    char *logo_path = ff_logo_path_gen(title);

    if (logo_path) {
        gtk_image_set_from_resource(GTK_IMAGE(image), logo_path);
        free(logo_path);
    }

    char *name = ff_simplify_title(title);
    cJSON *app = ff_get_application(name);
    cJSON *group = cJSON_GetObjectItem(app, "group");

    int learned_of_all = 0;
    for (int i = 0; i < cJSON_GetArraySize(group); ++i) {
        // Add categoryTitle
        cJSON *category = cJSON_GetArrayItem(group, i);
        cJSON *categoryTitle = cJSON_GetObjectItemCaseSensitive(category, "title");
        cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
        int learned = 0;
        for (int j = 0; j < cJSON_GetArraySize(shortcuts); ++j) {
            cJSON *shortcut = cJSON_GetArrayItem(shortcuts, j);
            cJSON *learn_stat = cJSON_GetObjectItemCaseSensitive(shortcut, "learned");
            learned += learn_stat->valueint;
            learned_of_all += learn_stat->valueint;
        }
        char progress[8];
        sprintf(progress, "%d/%d", learned, cJSON_GetArraySize(shortcuts));
        GtkWidget *row = ff_shortcut_list_row_new(categoryTitle->valuestring, progress);
        gtk_container_add(GTK_CONTAINER(list_topic), row);
    }

    cJSON *recent = cJSON_GetObjectItem(app, "recent");
    if (cJSON_GetArraySize(recent) > 0) {
        for (int i = cJSON_GetArraySize(recent) - 1; i >= 0; --i) {
            for (int j = 0; j < cJSON_GetArraySize(group); ++j) {
                const char *group_title =
                        cJSON_GetObjectItem(cJSON_GetArrayItem(group, j), "title")
                                ->valuestring;
                const char *recent_title_str = cJSON_GetArrayItem(recent, i)->valuestring;
                if (!strcmp(group_title, recent_title_str)) {
                    cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(
                            cJSON_GetArrayItem(group, j), "shortcuts");
                    int learned = 0;
                    for (int k = 0; k < cJSON_GetArraySize(shortcuts); ++k) {
                        cJSON *shortcut = cJSON_GetArrayItem(shortcuts, k);
                        cJSON *learn_stat =
                                cJSON_GetObjectItemCaseSensitive(shortcut, "learned");
                        learned += learn_stat->valueint;
                    }
                    char progress[8];
                    sprintf(progress, "%d/%d", learned, cJSON_GetArraySize(shortcuts));
                    GtkWidget *row = ff_shortcut_list_row_new(recent_title_str, progress);
                    gtk_container_add(GTK_CONTAINER(recent_list), row);
                }
            }
        }

    }

    g_signal_connect(quiz_button, "clicked", G_CALLBACK(quizButton_clicked_cb), app);
    g_signal_connect(GTK_WIDGET(list_topic), "row-activated",
                     G_CALLBACK(application_row_activated_cb), app);
    g_signal_connect(GTK_WIDGET(recent_list), "row-activated",
                     G_CALLBACK(application_row_activated_cb), app);

    free(name);

    gtk_stack_add_named(stack, GTK_WIDGET(scrolled_window), title);
    gtk_widget_show_all(GTK_WIDGET(scrolled_window));

    if (learned_of_all < 10)
        gtk_widget_set_visible(GTK_WIDGET(quiz_button), 0);
    if (cJSON_GetArraySize(recent) == 0){
        gtk_widget_set_visible(GTK_WIDGET(recent_list), 0);
        gtk_widget_set_visible(GTK_WIDGET(recent_title), 0);
    }
}
