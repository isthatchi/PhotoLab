#include "Image.h"
#include "Constants.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Black and White */
Image *BlackNWhite(Image *image){
    assert(image);
    int x, y, temp;

    for(y=0; y<ImageHeight(image); y++){
        for(x=0; x<ImageWidth(image); x++){
            temp = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y))/3;
            SetPixelR(image, x,y,temp);
            SetPixelG(image, x,y,temp);
            SetPixelB(image, x,y,temp);
        }
    }
    return image;
}

/* Reverse image color */
Image *Negative(Image *image){
    assert(image);
    int x,y;
    for(y=0; y<ImageHeight(image); y++){
        for(x=0; x<ImageWidth(image); x++){
            SetPixelR(image, x,y, MAX_PIXEL - GetPixelR(image,x,y));
            SetPixelG(image, x,y, MAX_PIXEL - GetPixelG(image,x,y));
            SetPixelB(image, x,y, MAX_PIXEL - GetPixelB(image,x,y)); 
        }
    }
    return image;
}
    
/* Color filter */
Image *ColorFilter(Image *image, int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b){
    assert(image);
    int x, y;
    replace_r = (replace_r > MAX_PIXEL) ? MAX_PIXEL : (replace_r < 0) ? 0 : replace_r;
    replace_g = (replace_g > MAX_PIXEL) ? MAX_PIXEL : (replace_g < 0) ? 0 : replace_g;
    replace_b = (replace_b > MAX_PIXEL) ? MAX_PIXEL : (replace_b < 0) ? 0 : replace_b;

    for(y=0; y<ImageHeight(image); y++){
        for(x=0; x<ImageWidth(image); x++){
            if(abs(GetPixelR(image, x, y) - target_r) <= threshold && abs(GetPixelG(image, x, y) - target_g) <= threshold && abs(GetPixelB(image, x, y) - target_b) <= threshold){
                SetPixelR(image, x,y, replace_r);
                SetPixelG(image, x,y, replace_g);
                SetPixelB(image, x,y, replace_b);
            }
        }
    }
    return image;
}

/* Edge detection */
Image *Edge(Image *image){
    assert(image);
    int x,y,m,n,a,b;
    Image *tempImage = NULL;
    tempImage = CreateImage(image->W,image->H);
    for(y=0; y<ImageHeight(image); y++){
        for(x=0; x<ImageWidth(image); x++){
            SetPixelR(tempImage, x,y, GetPixelR(image,x,y));
            SetPixelG(tempImage, x,y, GetPixelG(image,x,y));
            SetPixelB(tempImage, x,y, GetPixelB(image,x,y));
        }  
    }
    int sumR = 0;   
    int sumG = 0;
    int sumB = 0;
    for(y=1; y<ImageHeight(image) - 1; y++){
        for(x=1; x<ImageWidth(image) - 1; x++){
            for(n = -1; n <= 1; n++){
                for(m = -1; m<=1;m++){
                    a = (x + m >= image->W) ? image->W - 1 : (x + m < 0) ? 0 : x + m;
                    b = (y + n >= image->H) ? image->H - 1 : (y + n < 0) ? 0 : y + n;
                    sumR += GetPixelR(tempImage,x,y) - GetPixelR(tempImage,a,b);
                    sumG += GetPixelG(tempImage,x,y) - GetPixelG(tempImage,a,b);
                    sumB += GetPixelB(tempImage,x,y) - GetPixelB(tempImage,a,b);
                }
            }
            SetPixelR(image, x,y, (sumR > MAX_PIXEL) ? MAX_PIXEL: (sumR<0) ? 0: sumR);
            SetPixelG(image, x,y, (sumG > MAX_PIXEL) ? MAX_PIXEL: (sumG<0) ? 0: sumG);
            SetPixelB(image, x,y, (sumB > MAX_PIXEL) ? MAX_PIXEL: (sumB<0) ? 0: sumB);
            sumR = sumB = sumG = 0;
        }  
    }
    for(y=0; y<ImageHeight(image); y++){
        SetPixelR(image, 0, y, 0);
        SetPixelG(image, 0, y, 0);
        SetPixelB(image, 0, y, 0);

        SetPixelR(image, image -> W - 1, y, 0);
        SetPixelG(image, image -> W - 1, y, 0);
        SetPixelB(image, image -> W - 1, y, 0);

        }
    for(x=0; x<ImageWidth(image); x++){
        SetPixelR(image, x, 0, 0);
        SetPixelG(image, x, 0, 0);
        SetPixelB(image, x, 0, 0);

        SetPixelR(image, x, image -> H - 1, 0);
        SetPixelG(image, x, image -> H - 1, 0);
        SetPixelB(image, x, image -> H - 1, 0);
    }

    DeleteImage(tempImage);
    return image;
}

/* Flip image vertically */
Image *VFlip(Image *image){
    assert(image);
    int x,y;
    unsigned char r,g,b;
    for(y=0; y<ImageHeight(image)/2; y++){
        for(x=0; x<ImageWidth(image); x++){
            r = GetPixelR(image,x,image->H - 1 - y);
            g = GetPixelG(image,x,image->H - 1 - y);
            b = GetPixelB(image,x,image->H - 1 - y);

            SetPixelR(image, x, image -> H - 1 - y, GetPixelR(image,x,y));
            SetPixelG(image, x, image -> H - 1 - y, GetPixelG(image,x,y));
            SetPixelB(image, x, image -> H - 1 - y, GetPixelB(image,x,y));

            SetPixelR(image, x, y, r);
            SetPixelG(image, x, y, g);
            SetPixelB(image, x, y, b);
        }  
    }
    return image;
}
/* Mirror image vertically */
Image *HMirror(Image *image){
    assert(image);
    int x,y;
    for(y=0; y<ImageHeight(image); y++){
        for(x=0; x<ImageWidth(image)/2; x++){
            SetPixelR(image, image ->W - 1 - x,y, GetPixelR(image,x,y));
            SetPixelG(image, image ->W - 1 - x,y, GetPixelG(image,x,y));
            SetPixelB(image, image ->W - 1 - x,y, GetPixelB(image,x,y));
        }  
    }
    return image;
}

/* Zoom an image */
Image *AddBorder(Image *image, char *color, int border_width){

    assert(image);
    int x,y;
    int border_r = 255;
    int border_g = 255;
    int border_b = 255;
        if (!strcmp(color, "black")) {
                border_r = 0;
                border_g = 0;
                border_b = 0;
        } else if (!strcmp(color, "white")) {
                border_r = 255;
                border_g = 255;
                border_b = 255;
        } else if (!strcmp(color, "red")) {
                border_r = 255;
                border_g = 0;
                border_b = 0;
        } else if (!strcmp(color, "green")) {
                border_r = 0;
                border_g = 255;
                border_b = 0;
        } else if (!strcmp(color, "blue")) {
                border_r = 0;
                border_g = 0;
                border_b = 255;
        } else if (!strcmp(color, "yellow")) {
                border_r = 255;
                border_g = 255;
                border_b = 0;
        } else if (!strcmp(color, "cyan")) {
                border_r = 0;
                border_g = 255;
                border_b = 255;
        } else if (!strcmp(color, "pink")) {
                border_r = 255;
                border_g = 192;
                border_b = 203;
        } else if (!strcmp(color, "orange")) {
                border_r = 255;
                border_g = 165;
                border_b = 0;
        } else {
                printf("Unsupported color.\n");
                return ;
        }
        for(y=0; y<ImageHeight(image); y++){
            for(x=0; x<ImageWidth(image); x++){
                if((y < border_width) || (y > ImageHeight(image) - 1 - border_width) || (x < border_width) || (x > ImageWidth(image) - 1 - border_width)){
                    SetPixelR(image, x,y, border_r);
                    SetPixelG(image, x,y, border_g);
                    SetPixelB(image, x,y, border_b);
                    }
        }  
        }
    return image;
}
/* Shuffle an image */
Image *Shuffle(Image *image)
{
    assert(image);

    int block_width = ImageWidth(image) / SHUFF_WIDTH_DIV;
    int block_height = ImageHeight(image) / SHUFF_HEIGHT_DIV;

    int i, j, x, y;

    Image *temp_image = CreateImage(ImageWidth(image), ImageHeight(image));

    for (i = 0; i < SHUFF_HEIGHT_DIV / 2; i++) {
        for (j = 0; j < SHUFF_WIDTH_DIV; j++) {
            for (x = 0; x < block_width; x++) {
                for (y = 0; y < block_height; y++) {
                    SetPixelR(temp_image, x, y, GetPixelR(image, x + j * block_width, y + i * block_height));
                    SetPixelG(temp_image, x, y, GetPixelG(image, x + j * block_width, y + i * block_height));
                    SetPixelB(temp_image, x, y, GetPixelB(image, x + j * block_width, y + i * block_height));
                }
            }

            for (x = 0; x < block_width; x++) {
                for (y = 0; y < block_height; y++) {
                    SetPixelR(image, x + j * block_width, y + i * block_height, GetPixelR(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height)));
                    SetPixelG(image, x + j * block_width, y + i * block_height, GetPixelG(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height)));
                    SetPixelB(image, x + j * block_width, y + i * block_height, GetPixelB(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height)));

                    SetPixelR(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height), GetPixelR(temp_image, x, y));
                    SetPixelG(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height), GetPixelG(temp_image, x, y));
                    SetPixelB(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height), GetPixelB(temp_image, x, y));
                }
            }
        }
    }
    DeleteImage(temp_image);
    return image;
}
