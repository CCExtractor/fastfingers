#include "active-window.h"

#ifdef _X11_FOUND
#include <X11/Xatom.h>
#include <X11/Xlib.h>

int check_status(int status, unsigned long window) {
  if (status == BadWindow) {
    fprintf(stderr, "FF-ERROR: window id # 0x%lx does not exists!\n", window);
    return 1;
  }

  if (status != Success) {
    fprintf(stderr, "FF-ERROR :XGetWindowProperty failed!\n");
    return 2;
  }

  return 0;
}

char *get_active_from_xorg(void) {
  Display *display;
  Atom actual_type, filter_atom;
  int screen_number, actual_format, status, check;
  unsigned long root_window, active_window, nitems, bytes_after;
  unsigned char *prop;

  display = XOpenDisplay(NULL);
  if (!display) {
    fprintf(stderr, "FF-ERROR: Unable to open display '%s'\n",
            XDisplayName(NULL));
    return NULL;
  }

  screen_number = XDefaultScreen(display);
  root_window = XRootWindow(display, screen_number);
  filter_atom = XInternAtom(display, "_NET_ACTIVE_WINDOW", True);
  status = XGetWindowProperty(display, root_window, filter_atom, 0, 1024, False,
                              AnyPropertyType, &actual_type, &actual_format,
                              &nitems, &bytes_after, &prop);
  check = check_status(status, root_window);
  if (check) {
    XCloseDisplay(display);
    return NULL;
  }

  active_window = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);

  filter_atom = XInternAtom(display, "WM_CLASS", True);
  status = XGetWindowProperty(display, active_window, filter_atom, 0, 1024,
                              False, AnyPropertyType, &actual_type,
                              &actual_format, &nitems, &bytes_after, &prop);
  check = check_status(status, active_window);

  XCloseDisplay(display);

  if (check)
    return NULL;

  return (char *)prop;
}

#endif

char *get_active_from_gnome_shell(void) {
  GError *error = NULL;
  GDBusConnection *c = NULL;
  gchar *s, *parsed = NULL;

  c = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);

  if (!c) {
    g_printerr("Error connecting: %s\n", error->message);
    g_error_free(error);
    goto out;
  }

  GVariant *result = g_dbus_connection_call_sync(
      c, "org.gnome.Shell", "/org/gnome/Shell", "org.gnome.Shell", "Eval",
      g_variant_new("(s)", "global.get_window_actors().map(a=>a.meta_window)."
                           "find(w=>w.has_focus()).get_wm_class()"),
      NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
  if (!result) {
    g_printerr("Error: %s\n", error->message);
    g_error_free(error);
    goto out;
  }

  s = g_variant_print(result, TRUE);
  int len = strrchr(s, '"') - strchr(s, '"') - 1;
  parsed = malloc(len + 1);

  if (!parsed) {
    g_printerr("FF-Error: Couldn't parse the application name.\n");
    goto out;
  }

  memcpy(parsed, strchr(s, '"') + 1, len);
  parsed[len] = 0;

  g_free(s);

out:
  if (result)
    g_variant_unref(result);
  if (c)
    g_object_unref(c);

  return (char *)parsed;
}
