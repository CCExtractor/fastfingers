#include "ff-utils.h"

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
