#include "Constants.h"
#include "FileIO.h"
#include "Image.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

/* Get the R intensity of pixel (x, y) in image */
unsigned char GetPixelR(const Image *image, unsigned int x,  unsigned int y){
    assert(image);
    return image -> R[x + y * image->W];
}


/* Get the G intensity of pixel (x, y) in image */
unsigned char GetPixelG(const Image *image, unsigned int x,  unsigned int y){
    assert(image);
    return image -> G[x + y * image->W];
}

/* Get the B intensity of pixel (x, y) in image */
unsigned char GetPixelB(const Image *image, unsigned int x,  unsigned int y){
    assert(image);
    return image -> B[x + y * image->W];

}

/* Set the R intensity of pixel (x, y) in image to r */
void SetPixelR(Image *image, unsigned int x,  unsigned int y, unsigned char r){
    assert(image);
    image -> R[x + y * image->W] = r;


}

/* Set the G intensity of pixel (x, y) in image to g */
void SetPixelG(Image *image, unsigned int x,  unsigned int y, unsigned char g){
    assert(image);
    image -> G[x + y * image->W] = g;
}

/* Set the B intensity of pixel (x, y) in image to b */
void SetPixelB(Image *image, unsigned int x,  unsigned int y, unsigned char b){
    assert(image);
    image -> B[x + y * image->W] = b;

}

/* Allocate dynamic memory for the image structure and its R/G/B values */
/* Return the pointer to the image, or NULL in case of error */
Image *CreateImage(unsigned int Width, unsigned int Height){
    Image *image = NULL;
    image = malloc(sizeof(Image));
    if (!image){
        perror("Image cannot be created");
        exit(10);
    }

    image -> W = Width;
    image -> H = Height;

    image -> R = malloc(sizeof(unsigned char)*Width*Height);
    if(!image -> R){
        perror("Out of memory");
        exit(10);
    }

    image -> G = malloc(sizeof(unsigned char)*Width*Height);
    if(!image -> G){
        perror("Out of memory");
        exit(10);
    }

    image -> B = malloc(sizeof(unsigned char)*Width*Height);
    if(!image -> G){
        perror("Out of memory");
        exit(10);
    }
    return image;
}

/* Free the memory for the R/G/B values and Image structure */
void DeleteImage(Image *image){
    assert(image);
    free(image->R);
    free(image->G);
    free(image->B);
    image -> R = NULL;
    image -> G = NULL;
    image -> B = NULL;
    free(image);
    image = NULL;
}

/* Return the image's width in pixels */
unsigned int ImageWidth(const Image *image){
    assert(image);
    return image -> W;
}

/* Return the image's height in pixels */
unsigned int ImageHeight(const Image *image){
    assert(image);
    return image -> H;
}