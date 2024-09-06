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
#include "clix-ocr.hpp"

int main(int argc, const char * argv[]) 
{
  gfc_gc_init();
  clix::ocr::init("../tessdata");
  gfc_string_p text = clix::ocr::text("test/data/ocr.jpeg");
  
  
  gfc_string_print(text);
  gfc_string_free(text);
  
  gfc_gc_close();
  return 0;
}

