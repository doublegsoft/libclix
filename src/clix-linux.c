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
#include <X11/extensions/XTest.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clix-desktop.h"

struct clix_context_s
{
  Display* display;
};

static void 
clix_key_press(clix_context_t* ctx, KeySym keysym, int with_ctrl) 
{
  KeyCode keycode = XKeysymToKeycode(ctx->display, keysym);

  if (with_ctrl) {
    KeyCode ctrl = XKeysymToKeycode(ctx->display, XK_Control_L);
    XTestFakeKeyEvent(ctx->display, ctrl, True, 0);
  }

  XTestFakeKeyEvent(ctx->display, keycode, True, 0);
  XTestFakeKeyEvent(ctx->display, keycode, False, 0);

  if (with_ctrl) {
    KeyCode ctrl = XKeysymToKeycode(ctx->display, XK_Control_L);
    XTestFakeKeyEvent(ctx->display, ctrl, False, 0);
  }

  XFlush(ctx->display);
}

static void clix_ctrl_a(clix_context_t* ctx) 
{
  clix_key_press(ctx, XK_a, 1);
}

static void clix_ctrl_v(clix_context_t* ctx) 
{
  clix_key_press(ctx, XK_v, 1);
}

static 
void clix_click(clix_context_t* ctx, int button) 
{
  XTestFakeButtonEvent(ctx->display, button, True, CurrentTime);
  XTestFakeButtonEvent(ctx->display, button, False, CurrentTime);
  XFlush(ctx->display);
}

clix_context_t*
clix_context_init() 
{
  clix_context_t* ret = (clix_context_t*)malloc(sizeof(clix_context_t));
  ret->display = XOpenDisplay(NULL);
  return ret;
}

void 
clix_context_free(clix_context_t* ctx) 
{
  if (ctx == NULL) 
    return;
  XCloseDisplay(ctx->display);
}

void 
clix_move_to_point(clix_context_t* ctx, int x, int y) 
{
  XTestFakeMotionEvent(ctx->display, -1, x, y, CurrentTime);
  XFlush(ctx->display);
}

void 
clix_click_at_point(clix_context_t* ctx, int x, int y) 
{
  clix_move_to_point(ctx, x, y);
  usleep(200000);
  clix_click(ctx, 1);
}

void 
clix_dblclick_at_point(clix_context_t* ctx, int x, int y) 
{
  clix_click_at_point(ctx, x, y);
  usleep(150000);
  clix_click_at_point(ctx, x, y);
}

void 
clix_scroll(clix_context_t* ctx, int delta) 
{
  int button = delta > 0 ? 4 : 5;
  int count = abs(delta) / 50;

  for (int i = 0; i < count; i++) {
    clix_click(ctx, button);
    usleep(5000);
  }
}

void 
clix_paste_from_text(clix_context_t* ctx, const char* text) 
{
  clix_ctrl_a(ctx);
  usleep(1500000);
  clix_ctrl_v(ctx);
}

void 
clix_screen_capture(clix_context_t* ctx, const char* path) 
{
  Window root = DefaultRootWindow(ctx->display);
  XWindowAttributes gwa;

  XImage* image = XGetImage(ctx->display, root, 0, 0,
                            gwa.width, gwa.height,
                            AllPlanes, ZPixmap);

  if (!image) return;

  FILE* fp = fopen(path, "wb");
  if (!fp) return;

  fprintf(fp, "P6\n%d %d\n255\n", image->width, image->height);

  for (int y = 0; y < image->height; y++) {
    for (int x = 0; x < image->width; x++) {
      unsigned long pixel = XGetPixel(image, x, y);
      unsigned char r = (pixel & image->red_mask) >> 16;
      unsigned char g = (pixel & image->green_mask) >> 8;
      unsigned char b = (pixel & image->blue_mask);
      fputc(r, fp);
      fputc(g, fp);
      fputc(b, fp);
    }
  }

  fclose(fp);
  XDestroyImage(image);
}
