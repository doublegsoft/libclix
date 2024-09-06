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

#include <gfc.h>

#include "clix-t.hpp"
#include "clix-cv.hpp"

namespace clix
{

namespace cv
{

clix_img_p
init(const char* path);

clix_img_p
grayscale(clix_img_p img);

clix_img_p
blackwhite(clix_img_p img, uint threshold);

clix_img_p
bright(clix_img_p img, float brightness);

clix_img_p
blur(clix_img_p img);

void
save(const char *path, clix_img_p img);

void
match(const char *orig_path, const char *tpl_path, int *fx, int *fy);

void
info(clix_img_p img);

void
clean(clix_img_p img);

}; // cv

}; // clix
