#include "settings-page.h"

void ff_configure_autostart(int state) {
    char path[128];

    wordexp_t p;
    wordexp("~/.config/autostart", &p, 0);
    char **w = p.we_wordv;

    sprintf(path, "%s/org.ccextractor.Cheatsheet.desktop", w[0]);

    if (state) {
        const char *homedir = ff_get_home_dir();
        char model_path[128];
        sprintf(model_path, "%s/.fastfingers/model/org.ccextractor.Cheatsheet.desktop", homedir);
        FILE *source = fopen(model_path, "r");

        if (!source) {
            ff_error("Couldn't read desktop file at %s\n", model_path);
            return;
        }

        struct stat st = {0};

        if (stat(w[0], &st) == -1) {
            mkdir(w[0], 0700);
        }

        FILE *target = fopen(path, "w");

        if (!target) {
            ff_error("Couldn't create desktop file at ~/.config/autostart: %s\n",
                     strerror(errno));
            fclose(source);
            wordfree(&p);
            return;
        }

        char ch;
        while ((ch = fgetc(source)) != EOF)
            fputc(ch, target);

        fclose(source);
        fclose(target);
    } else {
        int ret = remove(path);
        if (ret)
            ff_error("Couldn't remove desktop file at ~/.config/autostart: %s\n",
                     strerror(errno));
    }
    wordfree(&p);
}

void ff_check_autostart_state(int state) {
    wordexp_t p;
    wordexp("~/.config/autostart/org.ccextractor.Cheatsheet.desktop", &p, 0);
    char **w = p.we_wordv;

    int exists = !access(w[0], F_OK);
    if (exists && !state)
        ff_configure_autostart(0);
    else if (!exists && state)
        ff_configure_autostart(1);

    wordfree(&p);
}

void settings_row_activated_cb(GtkListBox *box, GtkListBoxRow *row,
                               gpointer user_data) {
    GSettings *settings = (GSettings *) user_data;
    const char *row_name = gtk_widget_get_name(GTK_WIDGET(row));

    if (!strcmp(row_name, "autostart")) {
        GtkWidget *row_box = gtk_bin_get_child(GTK_BIN(row));
        GList *children = gtk_container_get_children(GTK_CONTAINER(row_box));
        GtkWidget *icon = (GtkWidget *) g_list_last(children)->data;

        gboolean new_state = !g_settings_get_boolean(settings, "autostart");
        // To-do: make it really starting auto
        g_settings_set_boolean(settings, "autostart", new_state);
        gtk_label_set_text(GTK_LABEL(icon), new_state ? "Enabled" : "Disabled");
        ff_configure_autostart(new_state);
    } else if (!strcmp(row_name, "reset_progress")) {
        GtkWidget *stack = ff_get_stack();
        ff_reset_progress_page_init(GTK_STACK(stack));
        ff_switch_page("reset-progress");
    } else if (!strcmp(row_name, "version")) {
        GtkWidget *window = ff_get_window();
        gtk_show_uri_on_window(
                GTK_WINDOW(window),
                "https://github.com/CCExtractor/fastfingers/blob/main/NEWS",
                GDK_CURRENT_TIME, NULL);
    } else if (!strcmp(row_name, "license")) {
        GtkWidget *window = ff_get_window();
        gtk_show_uri_on_window(
                GTK_WINDOW(window),
                "https://github.com/CCExtractor/fastfingers/blob/main/LICENSE",
                GDK_CURRENT_TIME, NULL);
    } else if (!strcmp(row_name, "support")) {
        GtkWidget *window = ff_get_window();
        gtk_show_uri_on_window(GTK_WINDOW(window),
                               "https://github.com/CCExtractor/fastfingers/issues",
                               GDK_CURRENT_TIME, NULL);
    } else if (!strcmp(row_name, "credits")) {
        ff_switch_page("credits");
    } else {
    }
}

void ff_settings_page_init(GtkStack *stack) {
    GtkWidget *temp = gtk_stack_get_child_by_name(stack, "settings");
    if (temp)
        gtk_container_remove(GTK_CONTAINER(stack), temp);
    GSettings *settings = g_settings_new("org.ccextractor.FastFingers");

    GtkBuilder *settings_page_builder = gtk_builder_new_from_resource(
            "/org/ccextractor/FastFingers/ui/settings-page.ui");

    GObject *main_box = gtk_builder_get_object(settings_page_builder, "main_box");
    GObject *autostart_label =
            gtk_builder_get_object(settings_page_builder, "autostart_label");
    GObject *list = gtk_builder_get_object(settings_page_builder, "list");
    GObject *version_label = gtk_builder_get_object(settings_page_builder, "version_label");

    gboolean current_state = g_settings_get_boolean(settings, "autostart");
    gtk_label_set_text(GTK_LABEL(autostart_label),
                       current_state ? "Enabled" : "Disabled");
    ff_check_autostart_state(current_state);

    g_signal_connect(GTK_WIDGET(list), "row-activated",
                     G_CALLBACK(settings_row_activated_cb), settings);

    gtk_label_set_label(GTK_LABEL(version_label), VERSION);
    gtk_stack_add_named(stack, GTK_WIDGET(main_box), "settings");
    gtk_widget_show_all(GTK_WIDGET(main_box));
}
