#include "fastfingers.h"

GObject *stack;
GObject *window;

typedef struct _page {
  const char *page;
  struct _page *previous;
} page;

page *current_page = NULL;

void activate(GtkApplication *app, gpointer user_data) {
  ff_init_css();

  srand(time(NULL));

  GtkBuilder *ffb_app = gtk_builder_new_from_resource(
      "/org/ccextractor/FastFingers/ui/fastfingers.ui");
  window = gtk_builder_get_object(ffb_app, "ff_window");
  gtk_window_set_application(GTK_WINDOW(window), app);

  stack = gtk_builder_get_object(ffb_app, "ff_stack");

  ff_home_page_init(GTK_STACK(stack));
  ff_settings_page_init(GTK_STACK(stack));
  ff_credits_page_init(GTK_STACK(stack));

  ff_switch_page("Home");

  gtk_widget_show(GTK_WIDGET(window));
}

void ff_switch_page(const char *page_name) {
  GtkWidget *page_widget =
      gtk_stack_get_child_by_name(GTK_STACK(stack), page_name);
  gtk_widget_show(page_widget);
  gtk_stack_set_visible_child_name(GTK_STACK(stack), page_name);

  page *new_page = malloc(sizeof(page));
  if (!new_page) {
    fprintf(stderr, "FF-Utils - Error: Couldn't allocate enough memory space!");
    return;
  }

  new_page->previous = current_page;
  new_page->page = page_name;
  current_page = new_page;
}

void ff_switch_previous(void) {
  if (!current_page)
    return;

  page *prev_page = current_page->previous;

  if (!prev_page)
    return;

  GtkWidget *page_widget;

  free(current_page);
  current_page = prev_page;

  page_widget =
      gtk_stack_get_child_by_name(GTK_STACK(stack), current_page->page);
  gtk_widget_show(page_widget);
  gtk_stack_set_visible_child_name(GTK_STACK(stack), current_page->page);
}

const char *ff_get_previous_page(void) {
  page *prev = current_page->previous;
  if (!prev)
    return NULL;
  return prev->page;
}

const char *ff_get_current_page(void) { return current_page->page; }

GtkWidget *ff_get_stack(void) { return (GtkWidget *)stack; }

GtkWidget *ff_get_window(void) { return (GtkWidget *)window; }
