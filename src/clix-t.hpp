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
#pragma once

#ifndef __CLIX_TYPE_HPP__
#define __CLIX_TYPE_HPP__

#include <gfc.h>

typedef struct clix_img_s {

  byte* bytes;

  uint width;

  uint height;

  uint chans;
}
clix_img_t;

typedef clix_img_t*   clix_img_p;

#endif /* __CLIX_TYPE_HPP__ */
