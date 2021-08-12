#include "quiz-result-page.h"

void ff_quiz_result_page_init(GtkStack *stack, const char *title) {
    GtkWidget *temp = gtk_stack_get_child_by_name(stack, title);
    if (temp)
        gtk_container_remove(GTK_CONTAINER(stack), temp);

    GtkBuilder *quiz_result_page_builder = gtk_builder_new_from_resource(
            "/org/ccextractor/FastFingers/ui/quiz-result-page.ui");

    GObject *scrolled_window = gtk_builder_get_object(
            quiz_result_page_builder, "quiz_result_scrolled_window");
    GObject *image = gtk_builder_get_object(quiz_result_page_builder, "image");
    GObject *list_topic =
            gtk_builder_get_object(quiz_result_page_builder, "list_topic");
    GObject *recent_list =
            gtk_builder_get_object(quiz_result_page_builder, "list_recent");
    GObject *recent_title =
            gtk_builder_get_object(quiz_result_page_builder, "recent_title");
    GObject *quiz_button =
            gtk_builder_get_object(quiz_result_page_builder, "quiz_button");

    char *logo_path = ff_logo_path_gen(title);

    if (logo_path) {
        gtk_image_set_from_resource(GTK_IMAGE(image), logo_path);
        free(logo_path);
    }


    gtk_stack_add_named(stack, GTK_WIDGET(scrolled_window), title);
    gtk_widget_show_all(GTK_WIDGET(scrolled_window));
}
