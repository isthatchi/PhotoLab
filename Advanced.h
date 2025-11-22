#ifndef ADVANCED_H_INCLUDED_
#define ADVANCED_H_INCLUDED_

#include "Constants.h"
#include "Image.h"

/* create a fisheye image */
double FishEyeLens(double coordinate, double radius, double distortion);

Image *FishEye(Image *image, double distortion_factor, double scaling_factor);

/* rotate and zoom the image */
Image *Rotate(Image *image, double Angle, double ScaleFactor,  int CenterX, int CenterY);

/* Posterize an image */
Image *Posterize(Image *image, int rbits, int gbits, int bbits);

/* Add motion blur to an image */
Image *MotionBlur(Image *image, unsigned char BlurAmount);

/* Crop */
Image *Crop(Image *image, int x, int y, int W, int H);

/* Resize */
Image *Resize(Image *image, int newWidth, int newHeight);

/* Watermark */
Image *Watermark(Image *image, const Image *watermark_image);

/* Rotate by 90 */
Image *RotateBy90(Image *image, int rotateDirection);
#endif /* ADVANCED_H_INCLUDED_ */
