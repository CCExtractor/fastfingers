#include <gtk/gtk.h>

#include "fastfingers.h"

int main (int argc, char **argv)
{
  return g_application_run(G_APPLICATION(fastfingers_new()), argc, argv);
}
