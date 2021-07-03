#include "cheatsheet.h"

/*
  static void
  search_changed_cb (GtkSearchEntry *entry,
  gpointer       *userdata)
  {
  const char *text;
  text = gtk_entry_get_text (GTK_ENTRY (entry));
  printf ("%s\n", text);
  }
*/

char *
get_active_window (void)
{
#ifdef _X11_FOUND
  return get_active_from_xorg ();
#else
  return get_active_from_gnome_shell ();
#endif
}

int strcmpic (const char *str1, const char *str2)
{
  int len = strlen(str1);
  if (len != strlen (str2))
    return 0;

  for (int i = 0; i < len; ++i)
    {
      if (toupper(str1[i]) != toupper(str2[i]))
	return 0;
    }
  
  return 1;
}

gboolean
focus_out_event_cb (GtkWidget *widget,
		    GdkEvent  *event,
		    gpointer  user_data)
{
  if (gtk_widget_get_visible (widget))
    gtk_widget_hide(widget);

  return 0;
}

gboolean
button_press_event_cb (GtkStatusIcon *status_icon,
		       GdkEvent      *event,
		       GtkWidget     *window)
{
  if (gtk_widget_get_visible (window))
    return 0;
  
  gtk_widget_show_all (window);

  return 0;
}

gboolean
delete_event_cb (GtkWidget *widget,
		 GdkEvent  *event,
		 gpointer   data)
{
  gtk_widget_hide(widget);
  return 1;
}

char *normalize_name (const char *name)
{
  if (strcmpic (name, "firefox"))
    return "firefox";

  return NULL;
}

void add_style_class (GtkWidget *widget, const char *class)
{
  GtkStyleContext *context = gtk_widget_get_style_context (widget);
  gtk_style_context_add_class (context, class);
}

GtkWidget *new_label_with_class (const char *text, const char *class)
{
  GtkWidget *label = gtk_label_new (text);
  add_style_class (label, class);
  return label;
}

GtkWidget *new_shortcut_box (void)
{
  GtkWidget *box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_widget_set_margin_left (box, 30);
  gtk_widget_set_margin_right (box, 30);  
  gtk_widget_set_hexpand (box, 1);
  return box;
}

GtkWidget *add_shortcut_title (GtkWidget *hbox, const char *shortcut_title)
{
  GtkWidget *shortcut_label = new_label_with_class (shortcut_title, "shortcut-title");
  gtk_label_set_line_wrap_mode (GTK_LABEL (shortcut_label), PANGO_WRAP_WORD_CHAR);
  gtk_label_set_justify (GTK_LABEL (shortcut_label), GTK_JUSTIFY_FILL);
  gtk_label_set_max_width_chars (GTK_LABEL (shortcut_label), 50);
  gtk_widget_set_halign (shortcut_label, GTK_ALIGN_START);
  gtk_widget_set_hexpand (shortcut_label, 1);
  gtk_label_set_line_wrap (GTK_LABEL (shortcut_label), 1);
  gtk_box_pack_start (GTK_BOX (hbox), shortcut_label, FALSE, TRUE, 0);
  return shortcut_label;
}

char *format_key (const char *str)
{
  int len = strlen (str);

  char *ret = NULL;

  if (len == 1)
    ret = g_strdup_printf ("%c", toupper(*str));

  else if (!strcmp (str, "Control"))
    ret = g_strdup ("Ctrl");

  else if (!strcmp (str, "BackSpace"))
    ret = g_strdup ("⌫");

  else if (!strcmp (str, "Delete"))
    ret = g_strdup ("Del");

  else if (!strcmp (str, "Right"))
    ret = g_strdup ("→");
  
  else if (!strcmp (str, "Left"))
    ret = g_strdup ("←");
  
  else if (!strcmp (str, "Up"))
    ret = g_strdup ("↑");
  
  else if (!strcmp (str, "Down"))
    ret = g_strdup ("↓");
  
  else if (!strcmp (str, "Page_Up") || !strcmp (str, "Page_Down"))
    {
      int idx = strchr (str, '_') - str;
      ret = g_strdup (str);
      ret[idx] = ' ';
    }

  else
    ret = g_strdup (str);
  
  return ret;
}

gboolean
update (GtkWidget *parent)
{
  static const char *last_window = NULL;
  char *active_window = get_active_window ();

  if (active_window)
    {
      const char *normalized = normalize_name (active_window);

      if ((!normalized) ||
	  (last_window && !strcmp (last_window, normalized)))
	goto out;

      last_window = normalized;

      GList *children = gtk_container_get_children (GTK_CONTAINER (parent));
      GtkWidget *former_box = (GtkWidget *)(g_list_last (children)->data);
      gtk_container_remove (GTK_CONTAINER (parent), former_box);

      GtkWidget *header = (GtkWidget *)(g_list_nth_data (children, 0));
      GList *header_children = gtk_container_get_children (GTK_CONTAINER (header));
      GtkImage *logo = (GtkImage *)(g_list_nth_data (header_children, 0));
      set_scaled_image (logo, normalized, 25);

      GtkLabel *app_label = (GtkLabel *)(g_list_nth_data (header_children, 1));
      char *app_title = g_strdup (normalized);
      app_title[0] = toupper (app_title[0]);
      gtk_label_set_text (app_label, app_title);
      free (app_title);
      
      GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
      gtk_box_pack_start (GTK_BOX (parent), vbox, FALSE, TRUE, 0);
      
      cJSON *app = ff_get_application (normalized);
      cJSON *group = cJSON_GetObjectItem(app, "group");
     
      for (int i = 0; i < cJSON_GetArraySize (group); ++i)
	{
	  cJSON *category = cJSON_GetArrayItem (group, i);
	  cJSON *category_title = cJSON_GetObjectItemCaseSensitive(category, "title");
	  GtkWidget *category_label = new_label_with_class (category_title->valuestring,
							    "category-title");
	  gtk_box_pack_start (GTK_BOX (vbox), category_label, FALSE, TRUE, 0);

	  cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
	  for (int j = 0; j < cJSON_GetArraySize (shortcuts); ++j)
	    {
	      cJSON *shortcut = cJSON_GetArrayItem (shortcuts, j);

	      GtkWidget *hbox = new_shortcut_box ();
	      cJSON *shortcut_title = cJSON_GetObjectItemCaseSensitive(shortcut, "title");
	      add_shortcut_title (hbox, shortcut_title->valuestring);
	      
	      cJSON *keys = cJSON_GetObjectItemCaseSensitive (shortcut, "keys");
	      if (cJSON_IsArray(cJSON_GetArrayItem (keys, 0)))
		keys = cJSON_GetArrayItem (keys, 0);
	      
	      for (int k = 0; k < cJSON_GetArraySize (keys); ++k)
		{
		  cJSON *key = cJSON_GetArrayItem (keys, k);
		  char *key_str = format_key (key->valuestring);
		  GtkWidget *key_label = new_label_with_class (key_str, "shortcut-key");

		  gtk_widget_set_valign (key_label, GTK_ALIGN_CENTER);
		  free (key_str);
		  gtk_box_pack_start (GTK_BOX (hbox), key_label, FALSE, TRUE, 0);
		}
	      
	      gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
	    }

	}

    out:
      free (active_window);
    }

  return 1;
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  ff_init_css ();

  GtkBuilder *builder = gtk_builder_new_from_resource ("/org/ccextractor/FastFingers/ui/cheatsheet.ui");
  //  window = gtk_application_window_new (app);
  
  GtkWidget *window = (GtkWidget *)gtk_builder_get_object (builder, "window");
  GtkWidget *main_box = (GtkWidget *)gtk_builder_get_object (builder, "main-box");
  gtk_window_set_title (GTK_WINDOW (window), "FastFingers Cheatsheet");
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ALWAYS);

  GtkWidget *entry = gtk_search_entry_new ();
  gtk_widget_set_halign (entry, GTK_ALIGN_CENTER);

  g_signal_connect(G_OBJECT(window), 
		   "delete-event", G_CALLBACK(delete_event_cb), NULL);
      
  /*
    GtkWidget *searchbar = (GtkWidget *)gtk_builder_get_object (builder, "searchbar");
    gtk_search_bar_connect_entry (GTK_SEARCH_BAR (searchbar), GTK_ENTRY (entry));
    gtk_search_bar_set_show_close_button (GTK_SEARCH_BAR (searchbar), FALSE);

    gtk_widget_set_size_request (entry, 550, 45);

    g_signal_connect (entry, "search-changed",
    G_CALLBACK (search_changed_cb), NULL);

  */
  
  GtkStatusIcon *status_icon = gtk_status_icon_new_from_file ("/usr/share/icons/hicolor/32x32/apps/fastfingers.png");

  g_signal_connect (status_icon, "button-press-event", G_CALLBACK (button_press_event_cb), window);
  g_signal_connect (window, "focus-out-event", G_CALLBACK (focus_out_event_cb), NULL);

  g_timeout_add (500,
		 G_SOURCE_FUNC (update),
		 main_box);
  
  gtk_main ();
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.ccextractor.Fastfingers.cheatsheet", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
