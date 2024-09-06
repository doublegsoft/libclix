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
#include <unistd.h>
#include "clix-cv.hpp"

int main(int argc, const char * argv[])
{
  gfc_gc_init();
  
  int x, y;
  clix::cv::match("test/data/screenshot.png", "test/data/copy.png", &x, &y);
  
  printf("x = %d, y = %d\n", x, y);
  gfc_gc_close();
  return 0;
}

