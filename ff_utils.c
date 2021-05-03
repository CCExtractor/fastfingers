#include "ff_utils.h"

void
ff_logo_path_gen (char *logo_path, const char *title)
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
