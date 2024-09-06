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
#include "clix-ocr.hpp"

#if defined(__clang__) && (__clang_major__ >= 14)
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

static tesseract::TessBaseAPI* tessapi = new tesseract::TessBaseAPI();
#endif

void clix::ocr::init(const char* tessdir)
{
#if defined(__clang__) && (__clang_major__ >= 14)  
  tessapi->Init(tessdir, "chi_sim");
#endif  
}

gfc_string_p clix::ocr::text(const char* img)
{
#if defined(__clang__) && (__clang_major__ >= 14)  
  char *outText;

  // Open input image with leptonica library
  Pix *image = pixRead(img);
  tessapi->SetImage(image);
  // Get OCR result
  outText = tessapi->GetUTF8Text();
  printf("OCR output:\n%s", outText);

  delete [] outText;
  pixDestroy(&image);
  
  gfc_string_p ret = gfc_string_new(outText);
  return ret;
#else
  return gfc_string_new("");  
#endif  
}
