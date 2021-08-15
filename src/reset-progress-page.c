#include "reset-progress-page.h"

GtkWidget *message, *info, *button_box;

void no_clicked_cb(GtkButton *button, gpointer user_data) {
    ff_switch_previous();
}

void yes_clicked_cb(GtkButton *button, gpointer user_data) {
    DIR *d;
    struct dirent *dir;
    const char *homedir = ff_get_home_dir();
    char path[128];
    sprintf(path, "%s/.fastfingers/applications", homedir);
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d))) {
            if (!g_str_has_suffix(dir->d_name, ".json"))
                continue;
            char *name = g_strndup(dir->d_name, strlen(dir->d_name) - 5);

            cJSON *app = ff_get_application(name);
            sprintf(path, "%s/.fastfingers/applications/%s.json", homedir, name);
            free(name);

            cJSON *group = cJSON_GetObjectItem(app, "group");

            for (int i = 0; i < cJSON_GetArraySize(group); ++i) {
                cJSON *category = cJSON_GetArrayItem(group, i);
                cJSON *shortcuts =
                        cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
                for (int j = 0; j < cJSON_GetArraySize(shortcuts); ++j) {
                    cJSON *shortcut = cJSON_GetArrayItem(shortcuts, j);
                    cJSON *learn_stat =
                            cJSON_GetObjectItemCaseSensitive(shortcut, "learned");
                    cJSON_SetIntValue(learn_stat, 0);
                }
            }

            cJSON *recent = cJSON_GetObjectItem(app, "recent");
            for (int i = cJSON_GetArraySize(recent) - 1; i >= 0; --i) {
                cJSON_DeleteItemFromArray(recent, i);
            }

            ff_write_JSON_to_file(app, path);
        }
        closedir(d);
    }

    gtk_label_set_text(GTK_LABEL(message), "Your progress has been reset.");
    gtk_label_set_text(GTK_LABEL(info), "You may continue.");
    gtk_widget_set_visible(button_box, 0);
}

void ff_reset_progress_page_init(GtkStack *stack) {
    GtkWidget *temp = gtk_stack_get_child_by_name(stack, "Reset-Progress");
    if (temp)
        gtk_container_remove(GTK_CONTAINER(stack), temp);

    GtkBuilder *reset_progress_builder = gtk_builder_new_from_resource(
            "/org/ccextractor/FastFingers/ui/reset-progress-page.ui");
    GObject *main_box =
            gtk_builder_get_object(reset_progress_builder, "main_box");
    GObject *no_btn = gtk_builder_get_object(reset_progress_builder, "no_btn");
    GObject *yes_btn = gtk_builder_get_object(reset_progress_builder, "yes_btn");
    message =
            GTK_WIDGET(gtk_builder_get_object(reset_progress_builder, "message"));
    info = GTK_WIDGET(gtk_builder_get_object(reset_progress_builder, "info"));
    button_box =
            GTK_WIDGET(gtk_builder_get_object(reset_progress_builder, "button_box"));

    g_signal_connect(GTK_WIDGET(no_btn), "clicked", G_CALLBACK(no_clicked_cb),
                     NULL);
    g_signal_connect(GTK_WIDGET(yes_btn), "clicked", G_CALLBACK(yes_clicked_cb),
                     NULL);

    gtk_stack_add_named(GTK_STACK(stack), GTK_WIDGET(main_box), "reset-progress");
    gtk_widget_show_all(GTK_WIDGET(main_box));
}
