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

#include "clix-desktop.h"

int 
main(int argc, const char* argv[])
{
  clix_context_t* ctx = clix_context_init();

  sleep(2);

  // clix_click_at_point(ctx, 500, 400);
  // sleep(1);

  // clix_dblclick_at_point(ctx, 500, 400);
  // sleep(1);

  // clix_scroll(ctx, 200);

  // clix_paste_from_text(ctx, "hello, clix");
  // sleep(2);

  clix_screen_capture(ctx, "screen.png");

  clix_context_free(ctx);
  return 0;
}