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
#include <string.h>

#include <opencv2/opencv.hpp>

#include "clix-cv.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static float blur_kernel[9] = {
  1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
  1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
  1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f
};

clix_img_p
clix::cv::init(const char* path)
{
  clix_img_p ret = (clix_img_p) gfc_gc_malloc(sizeof(clix_img_t), 1);
  memset(ret, 0, sizeof(clix_img_t));
  ret->bytes = stbi_load(path, (int*)&ret->width, (int*)&ret->height, (int*)&ret->chans, 0);
  return ret;
}

clix_img_p
clix::cv::grayscale(clix_img_p img)
{
  clix_img_p ret = (clix_img_p) gfc_gc_malloc(sizeof(clix_img_t), 1);
  ret->bytes = (byte*) malloc(sizeof(byte) * img->width * img->height * img->chans);
  ret->width = img->width;
  ret->height = img->height;
  ret->chans = img->chans;
  uint wh = img->width * img->height;
  for (int i = 0; i < wh; ++i) {
    unsigned char gray = (unsigned char)(
      0.3f * img->bytes[i * img->chans] +
      0.59f * img->bytes[i * img->chans + 1] +
      0.11f * img->bytes[i * img->chans + 2]
    );
    for (int j = 0; j < img->chans - 1; j++)
      ret->bytes[i * img->chans + j] = gray;
    ret->bytes[i * img->chans + 3] = img->bytes[i * img->chans + 3];
  }
  return ret;
}

clix_img_p
clix::cv::blackwhite(clix_img_p img, uint threshold)
{
  clix_img_p ret = (clix_img_p) gfc_gc_malloc(sizeof(clix_img_t), 1);
  ret->bytes = (byte*) malloc(sizeof(byte) * img->width * img->height * img->chans);
  ret->width = img->width;
  ret->height = img->height;
  ret->chans = img->chans;
  uint wh = img->width * img->height;
  for (int i = 0; i < wh; ++i) {
    unsigned char rgb = (unsigned char) ((
        img->bytes[i * img->chans] +
        img->bytes[i * img->chans + 1] +
        img->bytes[i * img->chans + 2]
    ) / 3);
    if (rgb >= threshold) {
      ret->bytes[i * img->chans + 0] = 255;
      ret->bytes[i * img->chans + 1] = 255;
      ret->bytes[i * img->chans + 2] = 255;
    } else {
      ret->bytes[i * img->chans + 0] = 0;
      ret->bytes[i * img->chans + 1] = 0;
      ret->bytes[i * img->chans + 2] = 0;
    }
    ret->bytes[i * img->chans + 3] = img->bytes[i * img->chans + 3];
  }
  return ret;
}

clix_img_p
clix::cv::bright(clix_img_p img, float brightness)
{
  clix_img_p ret = (clix_img_p) gfc_gc_malloc(sizeof(clix_img_t), 1);
  ret->bytes = (byte*) malloc(sizeof(byte) * img->width * img->height * img->chans);
  ret->width = img->width;
  ret->height = img->height;
  ret->chans = img->chans;
  uint wh = img->width * img->height * img->chans;
  for (int i = 0; i < wh; ++i)
    ret->bytes[i] = (byte)(255 * fmin(brightness * (img->bytes[i]/255.0f), 1.0f));

  return ret;
}

clix_img_p
clix::cv::blur(clix_img_p img)
{
  clix_img_p ret = (clix_img_p) gfc_gc_malloc(sizeof(clix_img_t), 1);
  ret->bytes = (byte*) malloc(sizeof(byte) * img->width * img->height * img->chans);
  ret->width = img->width;
  ret->height = img->height;
  ret->chans = img->chans;
  for (int y = 1; y < img->height - 1; y++) {
    for (int x = 1; x < img->width - 1; x++) {
      for (int c = 0; c < img->chans; c++) {
        float sum = 0.0f;
        for (int i = -1; i <= 1; i++) {
          for (int j = -1; j <= 1; j++) {
            sum += blur_kernel[(i + 1) * 3 + (j + 1)] *
                img->bytes[(y + i) * img->width * img->chans + (x + j) * img->chans + c];
          }
        }
        ret->bytes[y * img->width * img->chans + x * img->chans + c] = (byte) sum;
      }
    }
  }
  return ret;
}

void
clix::cv::match(const char* orig_path, const char* tpl_path, int* fx, int* fy)
{
  ::cv::Mat orig = ::cv::imread(orig_path, ::cv::IMREAD_COLOR);
  ::cv::Mat tpl = ::cv::imread(tpl_path, ::cv::IMREAD_COLOR);
  
  // Create a result matrix to store match values
  int resultCols = orig.cols - tpl.cols + 1;
  int resultRows = orig.rows - tpl.rows + 1;
  ::cv::Mat result(resultRows, resultCols, 4);

  // Perform template matching
  ::cv::matchTemplate(orig, tpl, result, ::cv::TM_CCOEFF_NORMED);
  
  float threshold = 0.8;
  ::cv::threshold(result, result, threshold, 1.0, ::cv::THRESH_TOZERO);

  // Find the locations of matches
  *fx = -1;
  *fy = -1;
  std::vector<::cv::Point> locations;
  while (true)
  {
    double minVal, maxVal;
    ::cv::Point minLoc, maxLoc;
    ::cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    if (maxVal >= threshold) 
    {
      if (maxLoc.y > *fy)
      {
        *fx = maxLoc.x;
        *fy = maxLoc.y;
      }
      locations.push_back(maxLoc);
      ::cv::floodFill(result, maxLoc, ::cv::Scalar(0), 0, ::cv::Scalar(.1), ::cv::Scalar(1.));
    } else
      break;
  }
  
//  // Find the location of the best match
//  ::cv::Point minLoc, maxLoc;
//  double minVal, maxVal;
//  ::cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, ::cv::Mat());
//  
//  fprintf(stdout, "min(%d,%d) max(%d,%d)\n", minLoc.x, minLoc.y, maxLoc.x, maxLoc.y);
//  double threshold = 0.90;
//  if (maxVal >= threshold)
//  {
//    *fx = maxLoc.x;
//    *fy = maxLoc.y;
//  }
//  else
//  {
//    *fx = -1;
//    *fy = -1;
//  }
}

void
clix::cv::save(const char* path, clix_img_p img)
{
  stbi_write_png(path, img->width, img->height, img->chans, img->bytes, img->width * img->chans);
}

void
clix::cv::info(clix_img_p img)
{
  fprintf(stdout, "width: %u\n"
                  "height: %u\n"
                  "channels: %d\n", img->width, img->height, img->chans);
}

void
clix::cv::clean(clix_img_p img)
{
  if (img == NULL)
    return;
  if (img->bytes != NULL)
    free(img->bytes);
  gfc_gc_free(img);
}
