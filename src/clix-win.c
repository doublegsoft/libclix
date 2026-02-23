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
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

#include "clix-desktop.h"

static void 
clix_key_press(WORD vk, int with_ctrl)
{
  INPUT inputs[4];
  ZeroMemory(inputs, sizeof(inputs));
  int count = 0;

  if (with_ctrl) {
    inputs[count].type = INPUT_KEYBOARD;
    inputs[count].ki.wVk = VK_CONTROL;
    count++;
  }

  inputs[count].type = INPUT_KEYBOARD;
  inputs[count].ki.wVk = vk;
  count++;

  inputs[count].type = INPUT_KEYBOARD;
  inputs[count].ki.wVk = vk;
  inputs[count].ki.dwFlags = KEYEVENTF_KEYUP;
  count++;

  if (with_ctrl) {
    inputs[count].type = INPUT_KEYBOARD;
    inputs[count].ki.wVk = VK_CONTROL;
    inputs[count].ki.dwFlags = KEYEVENTF_KEYUP;
    count++;
  }

  SendInput(count, inputs, sizeof(INPUT));
}

static void 
clix_ctrl_a()
{
  clix_key_press('A', 1);
}

static void 
clix_ctrl_v()
{
  clix_key_press('V', 1);
}

static void 
clix_click()
{
  INPUT inputs[2];
  ZeroMemory(inputs, sizeof(inputs));

  inputs[0].type = INPUT_MOUSE;
  inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

  inputs[1].type = INPUT_MOUSE;
  inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

  SendInput(2, inputs, sizeof(INPUT));
}

static int 
clix_save_as_png(const char* filename,
                 unsigned char* rgb,
                 int width,
                 int height)
{
  FILE* fp = fopen(filename, "wb");
  if (!fp) return -1;

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                            NULL, NULL, NULL);
  if (!png) return -1;

  png_infop info = png_create_info_struct(png);
  if (!info) return -1;

  if (setjmp(png_jmpbuf(png)))
  return -1;

  png_init_io(png, fp);

  png_set_IHDR(png,
               info,
               width,
               height,
               8,
               PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);

  png_write_info(png, info);

  png_bytep* rows = (png_bytep*)malloc(sizeof(png_bytep) * height);

  for (int y = 0; y < height; y++)
    rows[y] = rgb + y * width * 3;

  png_write_image(png, rows);
  png_write_end(png, NULL);

  free(rows);
  fclose(fp);
  png_destroy_write_struct(&png, &info);

  return 0;
}

void 
clix_move_to_point(clix_context_t* ctx, int x, int y)
{
  INPUT input;
  ZeroMemory(&input, sizeof(INPUT));

  input.type = INPUT_MOUSE;
  input.mi.dx = (x * 65535) / GetSystemMetrics(SM_CXSCREEN);
  input.mi.dy = (y * 65535) / GetSystemMetrics(SM_CYSCREEN);
  input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

  SendInput(1, &input, sizeof(INPUT));
}

void 
clix_click_at_point(clix_context_t* ctx, int x, int y)
{
  clix_move_to_point(ctx, x, y);
  Sleep(200);
  clix_click();
}

void 
clix_dblclick_at_point(clix_context_t* ctx, int x, int y)
{
  clix_click_at_point(ctx, x, y);
  Sleep(150);
  clix_click_at_point(ctx, x, y);
}

void 
clix_paste_from_text(clix_context_t* ctx, const char* text)
{
  OpenClipboard(NULL);
  EmptyClipboard();

  size_t len = strlen(text) + 1;
  HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);

  memcpy(GlobalLock(hMem), text, len);
  GlobalUnlock(hMem);

  SetClipboardData(CF_TEXT, hMem);
  CloseClipboard();

  Sleep(300);
  clix_ctrl_a();
  Sleep(300);
  clix_ctrl_v();
}

void 
clix_screen_capture(clix_context_t* ctx, const char* path)
{
  int width  = GetSystemMetrics(SM_CXSCREEN) * 2;
  int height = GetSystemMetrics(SM_CYSCREEN) * 2;

  HDC hScreen = GetDC(NULL);
  HDC hDC = CreateCompatibleDC(hScreen);
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);

  SelectObject(hDC, hBitmap);
  BitBlt(hDC, 0, 0, width, height, hScreen, 0, 0, SRCCOPY);

  BITMAPFILEHEADER bmfHeader;
  BITMAPINFOHEADER bi;

  bi.biSize = sizeof(BITMAPINFOHEADER);
  bi.biWidth = width;
  bi.biHeight = -height;
  bi.biPlanes = 1;
  bi.biBitCount = 24;
  bi.biCompression = BI_RGB;
  bi.biSizeImage = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed = 0;
  bi.biClrImportant = 0;

  DWORD dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;

  char* bmpBuffer = (char*)malloc(dwBmpSize);

  GetDIBits(hDC, hBitmap, 0, height, bmpBuffer,
            (BITMAPINFO*)&bi, DIB_RGB_COLORS);

  clix_save_as_png(path, bmpBuffer, width, height);

  // FILE* file = fopen(path, "wb");

  // bmfHeader.bfType = 0x4D42;
  // bmfHeader.bfSize =
  //   sizeof(BITMAPFILEHEADER) +
  //   sizeof(BITMAPINFOHEADER) +
  //   dwBmpSize;
  // bmfHeader.bfReserved1 = 0;
  // bmfHeader.bfReserved2 = 0;
  // bmfHeader.bfOffBits =
  //   sizeof(BITMAPFILEHEADER) +
  //   sizeof(BITMAPINFOHEADER);

  // fwrite(&bmfHeader, sizeof(BITMAPFILEHEADER), 1, file);
  // fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, file);
  // fwrite(bmpBuffer, dwBmpSize, 1, file);

  // fclose(file);

  free(bmpBuffer);
  DeleteObject(hBitmap);
  DeleteDC(hDC);
  ReleaseDC(NULL, hScreen);
}

