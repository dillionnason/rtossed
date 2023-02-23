#include <stdio.h>
#include <string.h>
#include "sh.h"
#include "sh_getline.h"

/*
 *  Basic shell function.
 */
int sh(void) 
{
  char buf[BUFFER_LEN]; 
  int chars;

  while (1) {
    printf("shell $ ");
    chars = sh_getline(buf, BUFFER_LEN);

    /* if strstr return a pointer to the beginning of buf, 
     * than echo + a space appears first */
    if (strstr(buf, "echo ") == (char *)buf) {
      if (chars > 4) {
        printf("%s\n\r", (char*)buf + 5);
      } 
    }
  }

  return chars;
}
