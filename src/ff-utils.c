#include "ff-utils.h"

void ff_init_css(void) {
  GtkCssProvider *provider = gtk_css_provider_new();
  GdkScreen *screen = gdk_screen_get_default();
  gtk_style_context_add_provider_for_screen(
      screen, GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_css_provider_load_from_resource(
      provider, "/org/ccextractor/FastFingers/FastFingers.css");

  g_object_unref(provider);
}

char *ff_simplify_title(const char *title) {
  char *simplified = malloc(sizeof(char) * 128);
  if (!simplified) {
    fprintf(stderr, "FF-Utils - Error: Couldn't allocate memory space for "
                    "simplified string!\n");
    return NULL;
  }

  char *cptr = simplified;
  while (*title) {
    if (*title != ' ')
      *cptr++ = tolower(*title);
    ++title;
  }

  return simplified;
}

char *ff_logo_path_gen(const char *title) {
  char *logo_path = malloc(sizeof(char) * 128);
  if (!logo_path) {
    fprintf(stderr, "FF-Utils - Error: Couldn't allocate memory space for "
                    "logo_path string!\n");
    return NULL;
  }

  sprintf(logo_path, "/org/ccextractor/FastFingers/logo/");
  char *cptr = logo_path + strlen(logo_path);
  while (*title) {
    if (*title != ' ')
      *cptr++ = tolower(*title);
    ++title;
  }
  sprintf(cptr, ".png");

  return logo_path;
}

struct _resizable_container {
  GtkWidget *image;
  GdkPixbuf *pixbuf;
};

gboolean ff_resize_image(GtkWidget *widget, GdkRectangle *allocation,
                         gpointer user_data) {
  int x, y, w, h;
  GdkPixbuf *pxbscaled;
  FFResizableContainer *container = (FFResizableContainer *)user_data;
  GtkWidget *image = (GtkWidget *)container->image;
  GdkPixbuf *pixbuf = (GdkPixbuf *)container->pixbuf;

  x = 0;
  y = 0;

  h = allocation->height;
  w = (gdk_pixbuf_get_width(pixbuf) * h) / gdk_pixbuf_get_height(pixbuf);

  pxbscaled = gdk_pixbuf_scale_simple(pixbuf, w, h, GDK_INTERP_BILINEAR);

  if (w < allocation->width) {
    x = (allocation->width - w) / 2;
    gtk_layout_move(GTK_LAYOUT(widget), image, x, y);
  }

  gtk_image_set_from_pixbuf(GTK_IMAGE(image), pxbscaled);

  g_object_unref(pxbscaled);

  return FALSE;
}

cJSON *ff_get_application(const char *name) {
  cJSON *app = NULL;
  char filepath[128], *data = NULL;
  long len, result;
  FILE *app_file;
  wordexp_t p;
  char **w;

  wordexp("~/.fastfingers/applications", &p, 0);
  w = p.we_wordv;

  sprintf(filepath, "%s/%s.json", w[0], name);
  wordfree(&p);

  app_file = fopen(filepath, "rb");

  if (!app_file) {
    fprintf(stderr, "FF-ERROR: Couldn't open file %s\n", filepath);
    goto out;
  }

  fseek(app_file, 0, SEEK_END);
  len = ftell(app_file);
  fseek(app_file, 0, SEEK_SET);

  data = (char *)malloc(len + 1);
  if (!data) {
    fprintf(stderr, "FF-ERROR: Couldn't allocate memory space!\n");
    goto out;
  }

  result = fread(data, 1, len, app_file);
  if (result != len) {
    fprintf(stderr, "FF-ERROR: Reading error occured!\n");
    goto out;
  }

  data[len] = '\0';

  app = cJSON_Parse(data);

  if (!app) {
    fprintf(stderr, "Error before: [%s]\n", cJSON_GetErrorPtr());
    goto out;
  }

out:
  if (app_file)
    fclose(app_file);
  if (data)
    free(data);

  return app;
}

void set_scaled_image(GtkImage *image, const char *title, int size) {
  int raw_width, raw_height, new_width, new_height;
  char *logo_path;
  GdkPixbuf *raw_pixbuf, *scaled_pixbuf;

  logo_path = ff_logo_path_gen(title);

  if (!logo_path)
    return;

  raw_pixbuf = gdk_pixbuf_new_from_resource(logo_path, NULL);

  raw_width = gdk_pixbuf_get_width(raw_pixbuf);
  raw_height = gdk_pixbuf_get_height(raw_pixbuf);

  if (raw_width > raw_height) {
    new_width = size;
    new_height = raw_height * ((double)new_width / raw_width);
  } else {
    new_height = size;
    new_width = raw_width * ((double)new_height / raw_height);
  }

  scaled_pixbuf = gdk_pixbuf_scale_simple(raw_pixbuf, new_width, new_height,
                                          GDK_INTERP_BILINEAR);
  gtk_image_set_from_pixbuf(image, scaled_pixbuf);

  free(logo_path);
}

char *normalize_keyval_name(const char *str) {
  int len = strlen(str);

  char *ret = NULL;

  if (len == 1)
    ret = g_strdup_printf("%c", toupper(*str));

  else if (!strcmp(str, "Control") || !strcmp(str, "Control_L") ||
           !strcmp(str, "Control_R"))
    ret = g_strdup("Ctrl");

  else if (!strcmp(str, "Alt") || !strcmp(str, "Alt_L") ||
           !strcmp(str, "Alt_R"))
    ret = g_strdup("Alt");

  else if (!strcmp(str, "Shift") || !strcmp(str, "Shift_L") ||
           !strcmp(str, "Shift_R"))
    ret = g_strdup("⇧");

  else if (!strcmp(str, "BackSpace"))
    ret = g_strdup("⌫");

  else if (!strcmp(str, "Return"))
    ret = g_strdup("⏎");

  else if (!strcmp(str, "Delete"))
    ret = g_strdup("Del");

  else if (!strcmp(str, "Right"))
    ret = g_strdup("→");

  else if (!strcmp(str, "Left"))
    ret = g_strdup("←");

  else if (!strcmp(str, "Up"))
    ret = g_strdup("↑");

  else if (!strcmp(str, "Down"))
    ret = g_strdup("↓");

  else if (!strcmp(str, "space"))
    ret = g_strdup("␣");

  else if (!strcmp(str, "Escape"))
    ret = g_strdup("Esc");

  else if (!strcmp(str, "Caps_Lock"))
    ret = g_strdup("⇪");

  else if (!strcmp(str, "quotedbl"))
    ret = g_strdup("\"");

  else if (!strcmp(str, "asterisk"))
    ret = g_strdup("*");

  else if (!strcmp(str, "minus"))
    ret = g_strdup("-");

  else if (!strcmp(str, "less"))
    ret = g_strdup("<");

  else if (!strcmp(str, "greater"))
    ret = g_strdup(">");

  else if (!strcmp(str, "period"))
    ret = g_strdup(".");

  else if (!strcmp(str, "comma"))
    ret = g_strdup(",");

  else if (!strcmp(str, "ISO_Level3_Shift"))
    ret = g_strdup("AltGr");

  else if (!strcmp(str, "Super_L") || !strcmp(str, "Super_R"))
    ret = g_strdup("Super");

  else if (!strcmp(str, "Insert"))
    ret = g_strdup("Ins");

  else if (!strcmp(str, "ISO_Level3_Shift"))
    ret = g_strdup("AltGr");

  else if (!strcmp(str, "ISO_Level3_Shift"))
    ret = g_strdup("AltGr");

  else if (!strcmp(str, "ISO_Level3_Shift"))
    ret = g_strdup("AltGr");

  else if (!strcmp(str, "Page_Up"))
    ret = g_strdup("Pg Up");

  else if (!strcmp(str, "Page_Down"))
    ret = g_strdup("Pg Dn");

  else if (!strcmp(str, "Scroll_Lock"))
    ret = g_strdup("⤓");

  else if (!strcmp(str, "Page_Down"))
    ret = g_strdup("Pg Dn");

  else if (!strcmp(str, "Num_Lock"))
    ret = g_strdup("⇭");

  else
    ret = g_strdup(str);

  return ret;
}

void null_check(void *check_ptr, const char *str, void (*fptr)(void *),
                void *free_arg) {
  if (check_ptr)
    return;

  if (fptr && free_arg)
    fprintf(stderr, "FF-ERROR: %s is null!\n", str);

  fptr(free_arg);
}

GtkWidget *ff_box_nth_child(GtkWidget *box, int idx) {
  GList *children = gtk_container_get_children(GTK_CONTAINER(box));
  GtkWidget *ret = g_list_nth(children, idx)->data;
  g_list_free(children);

  return ret;
}

GtkWidget *ff_box_first_child(GtkWidget *box) {
  GList *children = gtk_container_get_children(GTK_CONTAINER(box));
  GtkWidget *ret = g_list_first(children)->data;
  g_list_free(children);

  return ret;
}

GtkWidget *ff_box_last_child(GtkWidget *box) {
  GList *children = gtk_container_get_children(GTK_CONTAINER(box));
  GtkWidget *ret = g_list_last(children)->data;
  g_list_free(children);

  return ret;
}

void add_style_class(GtkWidget *widget, const char *class) {
  GtkStyleContext *context = gtk_widget_get_style_context(widget);
  gtk_style_context_add_class(context, class);
}

void remove_style_class(GtkWidget *widget, const char *class) {
  GtkStyleContext *context = gtk_widget_get_style_context(widget);
  gtk_style_context_remove_class(context, class);
}

static void remove_all_cb(GtkWidget *widget, GtkContainer *container) {
  gtk_container_remove(container, widget);
}

void ff_container_remove_all(GtkWidget *container) {
  if (!GTK_IS_CONTAINER(container)) {
    fprintf(stderr, "FF-ERRROR: %p is not a container to remove all children\n",
            container);
  }
  gtk_container_foreach(GTK_CONTAINER(container), remove_all_cb,
                        GTK_CONTAINER(container));
}
