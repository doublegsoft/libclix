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

static int 
clix_save_as_png(const char* filename, XImage* image)
{
  FILE* fp = fopen(filename, "wb");
  if (!fp) return -1;

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) return -1;

  png_infop info = png_create_info_struct(png);
  if (!info) return -1;

  if (setjmp(png_jmpbuf(png))) return -1;

  png_init_io(png, fp);

  png_set_IHDR(png, info,
               image->width,
               image->height,
               8,
               PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);

  png_write_info(png, info);

  png_bytep row = (png_bytep) malloc(3 * image->width);

  for (int y = 0; y < image->height; y++)
  {
    for (int x = 0; x < image->width; x++)
    {
      unsigned long pixel = XGetPixel(image, x, y);

      row[x*3 + 0] = (pixel & image->red_mask)   >> 16;
      row[x*3 + 1] = (pixel & image->green_mask) >> 8;
      row[x*3 + 2] = (pixel & image->blue_mask);
    }

    png_write_row(png, row);
  }

  png_write_end(png, NULL);

  free(row);
  fclose(fp);
  png_destroy_write_struct(&png, &info);

  return 0;
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
clix_enter(clix_context_t* ctx) 
{
  clix_key_press(ctx, XK_Return, 0);
}

void 
clix_paste_from_text(clix_context_t* ctx, const char* text) 
{
  FILE* pipe = popen("xclip -selection clipboard", "w");
  fputs(text, pipe);
  fclose(pipe);
  
  usleep(1500000);
  clix_ctrl_a(ctx);
  usleep(1500000);
  clix_ctrl_v(ctx);
}

void 
clix_screen_capture(clix_context_t* ctx, const char* path) 
{
  Window root = DefaultRootWindow(ctx->display);

  XWindowAttributes attrs;
  XGetWindowAttributes(ctx->display, root, &attrs);

  // XImage* image = XGetImage(ctx->display, root, 0, 0,
  //                           attrs.width, attrs.height,
  //                           AllPlanes, ZPixmap);
  XShmSegmentInfo shminfo;
  XImage* img = XShmCreateImage(
    ctx->display,
    DefaultVisual(ctx->display, DefaultScreen(ctx->display)),
    DefaultDepth(ctx->display, DefaultScreen(ctx->display)),
    ZPixmap,
    NULL,
    &shminfo,
    attrs.width,
    attrs.height
  );
  shminfo.shmid = shmget(IPC_PRIVATE,
                         img->bytes_per_line * img->height,
                         IPC_CREAT | 0777);
  shminfo.shmaddr = img->data = (char*)shmat(shminfo.shmid, 0, 0);
  shminfo.readOnly = False;

  XShmAttach(ctx->display, &shminfo);
  XShmGetImage(ctx->display, root, img, 0, 0, AllPlanes);
  clix_save_as_png(path, img);                          

  XDestroyImage(img);
}
