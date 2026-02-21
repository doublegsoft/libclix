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

#if defined(__APPLE__) && defined(__MACH__)
#import "clix-mac.h"
#elif defined(__linux)
#include "clix-desktop.h"
#endif

#include "clix-t.hpp"
#include "clix-ocr.hpp"
#include "clix-cv.hpp"
#include "clix-fmt.hpp"
