#include <gtk/gtk.h>
#include <string.h>

void btn_clicked(GtkWidget *widget, GtkEntry *entry);
void myCSS(void);
void createWind(GtkWidget **window, gint width, gint height);
void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name);

int main(int argc, char *argv[]){
    GtkWidget *window, *grid;
    GtkWidget *button1, *button2, *button3, *button4;

    gtk_init(&argc, &argv);
    myCSS();

    /*     Create the Window     */
    createWind(&window, 390, 290);

    /*     Create a Grid     */
    createGrid(&grid, &window, "myGrid");


    /*     Create a red Button    */
    button1 = gtk_button_new_with_label("Red");
    gtk_widget_set_name(button1, "myButton_red");
    gtk_widget_set_size_request(button1, 160, 130);
    g_object_set (button1, "margin", 5, NULL);

    /*     Create a yellow Button    */
    button2 = gtk_button_new_with_label("Yellow");
    gtk_widget_set_name(button2, "myButton_yellow");
    gtk_widget_set_size_request(button2, 160, 130);
    g_object_set (button2, "margin", 5, NULL);

    /*     Create a green Button    */
    button3 = gtk_button_new_with_label("Green");
    gtk_widget_set_name(button3, "myButton_green");
    gtk_widget_set_size_request(button3, 160, 130);
    g_object_set (button3, "margin", 5, NULL);

    /*     Create a blue Button    */
    button4 = gtk_button_new_with_label("Blue");
    gtk_widget_set_name(button4, "myButton_blue");
    gtk_widget_set_size_request(button4, 160, 130);
    g_object_set (button4, "margin", 5, NULL);


    /*     Putting all together      */
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button2, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button3, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button4, 1, 1, 1, 1);



    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}

void myCSS(void){
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *myCssFile = "mystyle.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
    g_object_unref (provider);
}

void createWind(GtkWidget **window, gint width, gint height){
    *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(*window), "MyApp");
    gtk_window_set_default_size(GTK_WINDOW(*window), width, height);
    gtk_window_set_resizable (GTK_WINDOW(*window), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(*window), 5);
    gtk_widget_set_name(*window, "window_main");
    g_signal_connect(*window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name){
    *grid = gtk_grid_new ();
    gtk_grid_set_row_homogeneous(GTK_GRID(*grid), FALSE);
    gtk_grid_set_column_homogeneous(GTK_GRID(*grid), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER (*grid), 15);
    gtk_grid_set_column_spacing(GTK_GRID(*grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(*grid), 5);
    gtk_widget_set_name(*grid, name);
    g_object_set (*grid, "margin", 22, NULL);
    gtk_container_add (GTK_CONTAINER (*window), *grid);
}

void btn_clicked(GtkWidget *widget, GtkEntry *entry){
    (void)widget;
    const gchar *gstrTexto;

    gstrTexto = gtk_entry_get_text(entry);
    g_print("%s\n", gstrTexto);
    gtk_editable_select_region(GTK_EDITABLE(entry) , 0, 3);
}

