#include <stdio.h>
#include "sh_getline.h"

/*
 *  Get a line of input from the serial interface.
 *  Return the number of characters received.
 */
int sh_getline(char *buf, uint16_t len)
{
  /* using a while loop so the user can keep typing, it just won't
   * enter the buffer past the end */
  int i = 0;
  int loop = 1;
  while (loop) {
    char ch = getchar();

    switch (ch) {
    case '\n':
    case '\r':        // handle newlines and carriage returns
      buf[i] = '\0';
      printf("\n\r");
      loop = 0;
      break;

    case '\b':        // handle backspace
      if (i > 0) {
        i -= 1;
        buf[i] = '\0';
        printf("\b \b");
      }
      break;

    default:          // handle all other characters
      if (i < len) {
        buf[i++] = ch;
      }
      printf("%c", ch);
    }
  }

  return i;
}
