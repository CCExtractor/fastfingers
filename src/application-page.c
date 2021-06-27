#include "application-page.h"

void
application_row_activated_cb (GtkListBox    *box,
			      GtkListBoxRow *row,
			      gpointer       user_data)
{
  gtk_list_box_unselect_row (box, row);
  GtkWidget *stack = ff_get_stack ();
  ff_practice_page_init (GTK_STACK(stack), (cJSON *) user_data,
			 ff_shortcut_list_row_get_left_text ((FFShortcutListRow *)row));
  ff_switch_page ("practice-page");
}

cJSON *ff_get_application(const char *name)
{
  cJSON *app = NULL;
  char filepath[128], *data;
  long len, result;
  FILE *app_file;
  
  sprintf(filepath, "/usr/share/fastfingers/applications/%s.json", name);
  app_file = fopen(filepath ,"rb");

  if (!app_file)
    {
      fprintf (stderr, "FF-ERROR: Couldn't open file %s\n", filepath);
      goto out;
    }
  
  fseek(app_file, 0, SEEK_END);
  len = ftell(app_file);
  fseek(app_file, 0, SEEK_SET);

  data = (char*)malloc(len + 1);
  if (!data)
    {
      fprintf (stderr, "FF-ERROR: Couldn't allocate memory space!\n");
      goto out;
    }
  
  result = fread(data, 1, len, app_file);
  if (result != len)
    {
      fprintf (stderr, "FF-ERROR: Reading error occured!\n");
      goto out;
    }
  
  data[len] = '\0';

  app = cJSON_Parse(data);

  if (!app)
    {
      fprintf(stderr, "Error before: [%s]\n", cJSON_GetErrorPtr());
      goto out;
    }

 out:
  if (app_file)
    fclose (app_file);
  if (data)
    free (data);
  
  return app;
}

void ff_application_page_init(GtkStack *stack, const char *title)
{
  GtkWidget *temp = gtk_stack_get_child_by_name (stack, title);
  if (temp)
    gtk_container_remove (GTK_CONTAINER (stack), temp);
    
  GtkBuilder *application_page_builder = gtk_builder_new_from_resource ("/org/ccextractor/FastFingers/ui/application-page.ui");
  
  GObject *scrolled_window = gtk_builder_get_object (application_page_builder, "application_scrolled_window");
  GObject *image = gtk_builder_get_object (application_page_builder, "image");
  GObject *list_topic = gtk_builder_get_object (application_page_builder, "list_topic");

  char *logo_path = ff_logo_path_gen(title);

  if (logo_path)
    {
      gtk_image_set_from_resource(GTK_IMAGE (image), logo_path);
      free(logo_path);
    }

  char *name = ff_simplify_title (title);
  char path[128];
  sprintf (path, "/usr/share/applications/%s.json", name);

  cJSON *app = ff_get_application (name);
  cJSON *group = cJSON_GetObjectItem(app, "group");

  for (int i = 0; i < cJSON_GetArraySize (group); ++i)
    {
      // Add title
      cJSON *category = cJSON_GetArrayItem (group, i);
      cJSON *title = cJSON_GetObjectItemCaseSensitive(category, "title");
      cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
      int learned = 0;
      for (int i = 0; i < cJSON_GetArraySize (shortcuts); ++i)
	{
	  cJSON *shortcut = cJSON_GetArrayItem (shortcuts, i);
	  cJSON *learn_stat = cJSON_GetObjectItemCaseSensitive(shortcut, "learned");
	  learned += learn_stat->valueint;
	}
      char progress[8];
      sprintf(progress, "%d/%d", learned, cJSON_GetArraySize (shortcuts));
      GtkWidget *row = ff_shortcut_list_row_new (title->valuestring, progress);
      gtk_container_add (GTK_CONTAINER (list_topic), row);

    }

  g_signal_connect (GTK_WIDGET (list_topic), "row-activated", G_CALLBACK (application_row_activated_cb), app);

  //free(app);
  free (name);
 
  gtk_stack_add_named(stack, GTK_WIDGET (scrolled_window), title);
  gtk_widget_show_all (GTK_WIDGET (scrolled_window));
}
