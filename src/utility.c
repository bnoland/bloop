#include "utility.h"

#include <stdlib.h>
#include <string.h>

char* strsep(char** stringp, const char* delim)
{
  if (*stringp == NULL) return NULL;
  char* p = *stringp + strcspn(*stringp, delim);
  char* token;
  if (*p == '\0') {
    token = *stringp;
    *stringp = NULL;
  } else {
    *p = '\0';
    token = *stringp;
    *stringp = p + 1;
  }
  return token;
}
