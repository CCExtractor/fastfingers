#include "ff-utils.h"

void
ff_init_css (void)
{
  GtkCssProvider *provider = gtk_css_provider_new ();
  GdkScreen *screen = gdk_screen_get_default ();
  gtk_style_context_add_provider_for_screen (screen,
					      GTK_STYLE_PROVIDER (provider),
					      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_css_provider_load_from_resource(provider, "/org/ccextractor/FastFingers/FastFingers.css");

  g_object_unref (provider);
}

char *
ff_simplify_title (const char *title)
{
  char *simplified = malloc(sizeof(char) * 128);
  if (!simplified)
    {
      fprintf(stderr, "FF-Utils - Error: Couldn't allocate memory space for simplified string!\n");
      return NULL;
    }
  
  char *cptr = simplified;
  while (*title)
    {
      if (*title != ' ')
	*cptr++ = tolower(*title);
      ++title;
    }

  return simplified;
}

char *
ff_logo_path_gen (const char *title)
{
  char *logo_path = malloc(sizeof(char) * 128);
  if (!logo_path)
    {
      fprintf(stderr, "FF-Utils - Error: Couldn't allocate memory space for logo_path string!\n");
      return NULL;
    }
  
  sprintf(logo_path, "/org/ccextractor/FastFingers/logo/");
  char *cptr = logo_path + strlen(logo_path);
  while (*title)
    {
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

gboolean ff_resize_image(GtkWidget *widget,
			 GdkRectangle *allocation,
			 gpointer user_data)
{
   int x,y,w,h;
   GdkPixbuf *pxbscaled;
   FFResizableContainer *container = (FFResizableContainer *) user_data;
   GtkWidget *image = (GtkWidget *) container->image; 
   GdkPixbuf *pixbuf= (GdkPixbuf *) container->pixbuf; 

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

   g_object_unref (pxbscaled);

   return FALSE;
}
