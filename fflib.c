#include "fflib.h"

void
logo_path_gen (char *logo_path, const char *title)
{
  sprintf(logo_path, "logos/");
  logo_path += strlen(logo_path);
  while (*title)
    {
      if (*title == ' ')
	{
	  ++title;
	  continue;
	}
      *logo_path++ = tolower(*title++);
    }
  sprintf(logo_path, ".png");
}

void
ff_init_css (void)
{
  GtkCssProvider *provider;
  GdkDisplay *display;
  GdkScreen *screen;

  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  const gchar *myCssFile = "style.css";
  GError *error = 0;

  gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
  g_object_unref (provider);
}
