#include "Constants.h"
#include "Image.h"
#include "Advanced.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

double FishEyeLens(double coordinate, double radius, double distortion)
{
        double distorted_radius;

        distorted_radius = 1.0 - distortion*(radius*radius);
        if (distorted_radius > -1e8 && distorted_radius < 1e-8) 
        {
                return coordinate;
        }
        return coordinate / distorted_radius;
}
/* fisheye */
Image *FishEye(Image *image, double distortion_factor, double scaling_factor){
    assert(image);
    int x, y, x_src, y_src;
    double normalX, normalY, radius, sourceX, sourceY;
    Image *SecondImage = NULL;
    SecondImage = CreateImage(ImageWidth(image), ImageHeight(image));

    for(y = 0; y < ImageHeight(image); y++){
        for(x = 0; x < ImageWidth(image); x++){
            normalX = (double) (2.0 * x - ImageWidth(image))/ (double) ImageWidth(image);
            normalY = (double) (2.0 * y - ImageHeight(image))/ (double) ImageHeight(image);

        radius = sqrt((normalX * normalX) + (normalY * normalY));

        sourceX = FishEyeLens(normalX, radius, distortion_factor);
        sourceY = FishEyeLens(normalY, radius, distortion_factor);

        sourceX /= scaling_factor;
        sourceY /= scaling_factor;


        x_src = ((sourceX + 1)*ImageWidth(image))/2.0;
        y_src = ((sourceY + 1)*ImageHeight(image))/2.0;

        if((0 <= x_src && x_src < ImageWidth(image)) && (0 <= y_src && y_src < ImageHeight(image))){
            SetPixelR(SecondImage, x, y, GetPixelR(image, x_src, y_src));
            SetPixelG(SecondImage, x, y, GetPixelG(image, x_src, y_src));
            SetPixelB(SecondImage, x, y, GetPixelB(image, x_src, y_src));
        } else {
            SetPixelR(SecondImage, x, y, 0);
            SetPixelG(SecondImage, x, y, 0);
            SetPixelB(SecondImage, x, y, 0);
        }
    }
}
    for(y = 0; y < ImageHeight(image); y++){
        for(x = 0; x < ImageWidth(image); x++){
            SetPixelR(image, x, y, GetPixelR(SecondImage, x,y));
            SetPixelG(image, x, y, GetPixelG(SecondImage, x,y));
            SetPixelB(image, x, y, GetPixelB(SecondImage, x,y));
        }
    }
    DeleteImage(SecondImage);
    return image;
}

/* rotate */
Image *Rotate(Image *image, double Angle, double ScaleFactor, int CenterX, int CenterY){
    Image *RotatedImage = CreateImage(image->W, image->H);
    int x, y;
    int x2, y2;
    const double pi = 3.141592653589793238462024;
    double Radian;
    double ScaleX, ScaleY;
    double M[2][2];

    Radian = -Angle/360.0 * 2*pi;
    ScaleX = ScaleFactor;
    ScaleY = ScaleFactor;
    M[0][0] = cos(Radian)/ScaleX;
    M[0][1] = -sin(Radian)/ScaleX;
    M[1][0] = sin(Radian)/ScaleY;
    M[1][1] = cos(Radian)/ScaleY;

    for(y = 0; y < ImageHeight(image); y++){
        for(x = 0; x < ImageWidth(image); x++){
            x2 = (int)((x - CenterX) * M[0][0] + (y - CenterY) * M[0][1] + CenterX);
            y2 = (int)((x - CenterX) * M[1][0] + (y - CenterY) * M[1][1] + CenterY);
            if ((x2 >= 0) && (x2 < image->W) && (y2 >= 0) && (y2 < image->H)){
                SetPixelR(RotatedImage, x, y, GetPixelR(image, x2, y2));
                SetPixelG(RotatedImage, x, y, GetPixelG(image, x2, y2));
                SetPixelB(RotatedImage, x, y, GetPixelB(image, x2, y2));
            } else {
                SetPixelR(RotatedImage, x, y, 0);
                SetPixelG(RotatedImage, x, y, 0);
                SetPixelB(RotatedImage, x, y, 0);
            }
        }
    }

    for(y = 0; y < ImageHeight(image); y++){
        for(x = 0; x < ImageWidth(image); x++){
            SetPixelR(image, x, y, GetPixelR(RotatedImage, x, y));
            SetPixelG(image, x, y, GetPixelG(RotatedImage, x, y));
            SetPixelB(image, x, y, GetPixelB(RotatedImage, x, y));
        }
    }
    DeleteImage(RotatedImage);
    return image;
}

/* Posterize an image */
Image *Posterize(Image *image, int rbits, int gbits, int bbits){
    assert(image);
    int x,y;
    for(x=0; x < ImageWidth(image); x++){
        for(y = 0; y < ImageHeight(image); y++){
            unsigned char one = 1;
            int i;
            for (i = 0; i < rbits - 1; i++){
                SetPixelR(image, x , y , GetPixelR(image, x, y) | one);
                one <<= 1;
            }
            one = ~one;
            SetPixelR(image, x, y, GetPixelR(image, x,y) & one);

            one = 1;
            for (i = 0; i <  gbits - 1; i++){
                SetPixelG(image, x, y , GetPixelG(image, x, y) | one);
                one <<= 1;
            }
            one = ~one;
            SetPixelG(image, x, y, GetPixelG(image, x,y) & one);


            one = 1;
            for (i = 0; i < bbits - 1; i++){
                SetPixelB(image, x, y, GetPixelB(image, x, y) | one);
                one <<= 1;
            }
            one = ~one;
            SetPixelB(image, x, y, GetPixelB(image, x,y) & one);
        }
    }
    return image;
}

/*Motion Blur*/
Image *MotionBlur(Image *image, unsigned char BlurAmount){
    assert(image);
    int x,y, k;
    double redBlur , greenBlur , blueBlur;
    double sumR, sumG, sumB;
    int n;

    for (x = 0; x < ImageWidth(image)-1; x++)
    {
        for (y = 0; y < ImageHeight(image); y++)
        {
            redBlur = GetPixelR(image, x, y);
            greenBlur = GetPixelG(image, x, y);
            blueBlur = GetPixelB(image, x, y);
            sumR = 0.0;
            sumG= 0.0;
            sumB = 0.0;
            n = 0;
            for (k = 1; k<=BlurAmount ; k++)
            {
                if ((x+k) < ImageWidth(image))
                {
                    sumR = sumR + GetPixelR(image, x+k, y);
                    sumG = sumG + GetPixelG(image, x+k, y);
                    sumB = sumB + GetPixelB(image, x+k, y);
                    n = k;
                }
                else
                {
                    break;
                }
            }
            redBlur = redBlur * 0.5 + sumR * 0.5/n;
            greenBlur = greenBlur * 0.5 + sumG * 0.5/n;
            blueBlur = blueBlur * 0.5 + sumB * 0.5/n;
            SetPixelR(image, x, y, redBlur);
            SetPixelG(image, x, y, greenBlur);
            SetPixelB(image, x, y, blueBlur);
        }
    }
    return(image);
}


/*Crop*/
Image *Crop(Image *image, int x, int y, int W, int H){
    assert(image);
    int i,j; 

    if(x + W > ImageWidth(image)){
        W = ImageWidth(image) - x;
    }

    if(y + H > ImageHeight(image)){
        H = ImageHeight(image) - y;
    }
    Image *croppedImage = CreateImage(W, H);

    for(i = 0; i < ImageWidth(croppedImage); i++){
        for(j = 0; j < ImageHeight(croppedImage); j++){
            SetPixelR(croppedImage, i, j, GetPixelR(image, x + i, y + j));
			SetPixelG(croppedImage, i, j, GetPixelG(image, x + i, y + j));
			SetPixelB(croppedImage, i, j, GetPixelB(image, x + i, y + j));
        }
    }
    DeleteImage(image);
    return croppedImage;
}





/* Resize */
Image *Resize(Image *image, int newWidth, int newHeight){
    assert(image);
	int x, y;
	int original_width, original_height;
    Image *scaleImage = NULL;
    scaleImage = CreateImage(newWidth,newHeight);

    for(x = 0; x < ImageWidth(scaleImage); x++){
        for(y = 0; y < ImageHeight(scaleImage); y++){
            original_width = (int) x * ImageWidth(image) / newWidth;
            original_height = (int) y * ImageHeight(image) / newHeight;

            SetPixelR(scaleImage, x, y, GetPixelR(image, original_width, original_height));
            SetPixelG(scaleImage, x, y, GetPixelG(image, original_width, original_height));
            SetPixelB(scaleImage, x, y, GetPixelB(image, original_width, original_height));
        }

    }
    DeleteImage(image);
    return scaleImage;
}







/* Watermark */
Image *Watermark(Image *image, const Image *watermark_image){
    assert(image);
    int x,y;
    int watermarkR, watermarkG, watermarkB;
    int tempR, tempG, tempB;

    for(x = 0; x < ImageWidth(image); x++){
        for(y = 0; y < ImageHeight(image); y++){
            watermarkR = GetPixelR(watermark_image, x % ImageWidth(watermark_image), y % ImageHeight(watermark_image));
			watermarkG = GetPixelG(watermark_image, x % ImageWidth(watermark_image), y % ImageHeight(watermark_image));
			watermarkB = GetPixelB(watermark_image, x % ImageWidth(watermark_image), y % ImageHeight(watermark_image));

            if ((watermarkR + watermarkG + watermarkB) == 0)
			{
				tempR = GetPixelR(image, x, y) * 1.45;
				if(tempR > 255)
				{
					tempR = 255;   
				}    
				SetPixelR(image, x, y, tempR);

				tempG = GetPixelG(image, x, y) * 1.45;
				if(tempG > 255)
				{
					tempG = 255;
				}
				SetPixelG(image, x, y, tempG);

				tempB = GetPixelB(image, x, y) * 1.45; 
				if(tempB > 255)
				{
					tempB = 255;
				}
				SetPixelB(image, x, y, tempB);
			}
		}
	}
	return image;    
}	





/* Rotate by 90 */
Image *RotateBy90(Image *image, int rotateDirection){
    assert(image); 

    int originalWidth = ImageWidth(image);
    int originalHeight = ImageHeight(image);

    Image *rotatedImage = CreateImage(originalHeight, originalWidth);
    if (!rotatedImage) {
        return NULL;
    }

    for (int y = 0; y < originalHeight; y++) {
        for (int x = 0; x < originalWidth; x++) {
            int newX, newY;
            if (rotateDirection == 0) {
                newX = originalHeight - 1 - y;
                newY = x;
            } else {
                newX = y;
                newY = originalWidth - 1 - x;
            }
            SetPixelR(rotatedImage, newX, newY, GetPixelR(image, x, y));
            SetPixelG(rotatedImage, newX, newY, GetPixelG(image, x, y));
            SetPixelB(rotatedImage, newX, newY, GetPixelB(image, x, y));
        }
    }

    DeleteImage(image);

    return rotatedImage;
}
 /* ADVANCED_H_INCLUDED_ */
