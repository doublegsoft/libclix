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
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

#include "clix-desktop.hpp"
#include "clix-cv.hpp"

ClixDesktopSimulator::ClixDesktopSimulator(const std::string& dir)
  : workdir(dir) 
{
  this->ctx = clix_context_init();
}

ClixDesktopSimulator::~ClixDesktopSimulator() 
{
  clix_context_free(this->ctx);
}

void 
ClixDesktopSimulator::moveTo(int x, int y) 
{
  clix_move_to_point(this->ctx, x, y);
}

void 
ClixDesktopSimulator::clickAt(int x, int y) 
{
  clix_click_at_point(this->ctx, x, y);
}

void ClixDesktopSimulator::dblclickAt(int x, int y) 
{
  clickAt(x, y);
  usleep(100000);
  clickAt(x, y);
}
