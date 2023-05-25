#include <stdio.h>
#include <iostream> //Input and Output Stream
#include <string>
#include <math.h>
#include "header/chargraphic.h"




int main(int argc, char *args[])
{
  
  charpoin point = {CHAR_CANVOS_WIDTH-1,CHAR_CANVOS_HEIGHT-1};
  type_backcolor bcolor = type_backcolor::bred;
  type_fontcolor fcolor = type_fontcolor::fred;
  // system("ls -la");
  clearScreen();
  hideCursor();
  setFontColor(fcolor);
  lineto(point);
  while (true)
  {
    char u = scanKeyboard();
    if (u == 27)
      break;
    if (u != -1)
      printf("%c", u);
    else
    {
      clearScreen();
      hideCursor();
    }
  }
  return 0;
}
