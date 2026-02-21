#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Display *display;

/* ===================== INIT ===================== */

void sim_init() {
  display = XOpenDisplay(NULL);
  if (!display) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }
}

/* ===================== MOUSE ===================== */

void move_to(int x, int y) {
  XTestFakeMotionEvent(display, -1, x, y, CurrentTime);
  XFlush(display);
}

void click(int button) {
  XTestFakeButtonEvent(display, button, True, CurrentTime);
  XTestFakeButtonEvent(display, button, False, CurrentTime);
  XFlush(display);
}

void left_click(int x, int y) {
  move_to(x, y);
  usleep(200000);
  click(1);
}

void double_click(int x, int y) {
  left_click(x, y);
  usleep(150000);
  left_click(x, y);
}

/* ===================== SCROLL ===================== */

void scroll(int delta) {
  int button = delta > 0 ? 4 : 5;
  int count = abs(delta) / 50;

  for (int i = 0; i < count; i++) {
    click(button);
    usleep(5000);
  }
}

/* ===================== KEYBOARD ===================== */

void key_press(KeySym keysym, int with_ctrl) {
  KeyCode keycode = XKeysymToKeycode(display, keysym);

  if (with_ctrl) {
    KeyCode ctrl = XKeysymToKeycode(display, XK_Control_L);
    XTestFakeKeyEvent(display, ctrl, True, 0);
  }

  XTestFakeKeyEvent(display, keycode, True, 0);
  XTestFakeKeyEvent(display, keycode, False, 0);

  if (with_ctrl) {
    KeyCode ctrl = XKeysymToKeycode(display, XK_Control_L);
    XTestFakeKeyEvent(display, ctrl, False, 0);
  }

  XFlush(display);
}

/* Ctrl+A */
void select_all() {
  key_press(XK_a, 1);
}

/* Ctrl+V */
void paste() {
  key_press(XK_v, 1);
}

/* ===================== SCREENSHOT ===================== */

int capture_screen(const char* path) {
  Window root = DefaultRootWindow(display);
  XWindowAttributes gwa;
  XGetWindowAttributes(display, root, &gwa);

  XImage *image = XGetImage(display, root, 0, 0,
                            gwa.width, gwa.height,
                            AllPlanes, ZPixmap);

  if (!image) return -1;

  FILE *fp = fopen(path, "wb");
  if (!fp) return -1;

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
  return 0;
}

/* ===================== CLEANUP ===================== */

void sim_destroy() {
  XCloseDisplay(display);
}

/* ===================== DEMO ===================== */

int main() {
  sim_init();

  sleep(2);

  left_click(500, 400);
  sleep(1);

  double_click(500, 400);
  sleep(1);

  select_all();
  sleep(1);

  paste();
  sleep(1);

  scroll(200);

  capture_screen("screen.ppm");

  sim_destroy();
  return 0;
}