/*
**    ,,    ,,  ,,                 ,,    ,,
**  `7MM    db *MM               `7MM    db
**    MM        MM                 MM
**    MM  `7MM  MM,dMMb.   ,p6"bo  MM  `7MM  `7M'   `MF'
**    MM    MM  MM    `Mb 6M'  OO  MM    MM    `VA ,V'
**    MM    MM  MM     M8 8M       MM    MM      XMX
**    MM    MM  MM.   ,M9 YM.    , MM    MM    ,V' VA.
**  .JMML..JMML.P^YbmdP'   YMbmd'.JMML..JMML..AM.   .MA.
*/
#include <windows.h>
#include "clix-desktop.h"

int main()
{
  Sleep(2000);

  clix_click_at_point(NULL, 500, 500);
  clix_paste_from_text(NULL, "Hello from MinGW!");
  clix_screen_capture(NULL, "screenshot.bmp");

  return 0;
}