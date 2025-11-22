#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "DIPs.h"
#include "Advanced.h"
#include "FileIO.h"
#include "Constants.h"
#include "Test.h"
#include "Image.h"



void PrintMenu();

int main() {
#ifdef DEBUG
        AutoTest();
#else
        int r24;
        int option;                     /*   user input option   */
        char fname[SLEN];               /* input file name */
        char colorOption[SLEN];


	PrintMenu();
	printf("Please make your choice: ");
	scanf("%d", &option);

	Image *image = NULL;
	Image *watermark_image = NULL;
	r24 = -1;


	/* ColorFilter */
	int target_r, target_g, target_b, threshold;
	double factor_r, factor_g, factor_b;
	
	/* AddBorder */
	int border_width;

	/* Crop */
	int x, y, W, H;

	/* Resize */
	int newWidth, newHeight;

	/* MotionBlur */
	int BlurAmount;

	/* Fisheye */
	double distortion_factor, scaling_factor;

	/* Rotate */
	double Angle, ScaleFactor;  
	int CenterX, CenterY;

	/* Posterize */
	unsigned char rbits, gbits, bbits;



	while (option != 20) {
		if (option == 1) {
			if(image)
			{
				DeleteImage(image);
			}
			printf("Please input the file name to load: ");
			scanf("%s", fname);
			image = LoadImage(fname);
			if(image)
			{
				r24 = SUCCESS;
			}
		}
		/* menu item 2 - 18 requires image is loaded first */
		else if (option >= 2 && option < 19) {
			if (r24 != SUCCESS)   {
				printf("No image to process!\n");
			}
			/* now image is loaded */
			else {
				switch(option) {
					case 2:
						printf("Please input the file name to save: ");
						scanf("%s", fname);
						SaveImage(fname, image);
						break;
					case 3:
						BlackNWhite(image);
						printf("\"Black & White\" operation is done!\n");
						break;
					case 4:
						Negative(image);
						printf("\"Negative\" operation is done!\n");
						break;
					case 5:
						printf("Enter Red   component for the target color: ");
						scanf("%d", &target_r);
						printf("Enter Green component for the target color: ");
						scanf("%d", &target_g);
						printf("Enter Blue  component for the target color: ");
						scanf("%d", &target_b);
						printf("Enter threshold for the color difference: ");
						scanf("%d", &threshold);
						printf("Enter value for Red component in the target color: ");
						scanf("%lf", &factor_r);
						printf("Enter value for Green component in the target color: ");
						scanf("%lf", &factor_g);
						printf("Enter value for Blue  component in the target color: ");
						scanf("%lf", &factor_b);

						ColorFilter(image, target_r, target_g, target_b, threshold, factor_r, factor_g, factor_b);
						printf("\"Color Filter\" operation is done!\n");
						break;
					case 6:
						Edge(image);
						printf("\"Edge\" operation is done!\n");
						break;
					case 7:
						Shuffle(image);
						printf("\"Shuffle\" operation is done!\n");
						break;
					case 8:
						VFlip(image);
						printf("\"VFlip\" operation is done!\n");
						break;
					case 9:
						HMirror(image);
						printf("\"Horizontal Mirror\" operation is done!\n");
						break;
					case 10:
						printf("Enter border width:");
						scanf("%d", &border_width);
						printf("Available border colors : black, white, red, green, blue, yellow, cyan, pink, orange\n");
						printf("Select border color from the options: ");
						scanf("%s",colorOption);
						AddBorder(image, colorOption, border_width);
						printf("\"Border\" operation is done!\n");
						break;
					case 11:
						printf("Enter a value for distortion factor:");
                        scanf("%lf", &distortion_factor);
                        printf("Enter a value for scaling factor:");
                        scanf("%lf", &scaling_factor);
                        FishEye(image, distortion_factor, scaling_factor);
                        printf("\"FishEye\" operation is done!\n");
                        break;
					case 12:
						printf("Enter the number of posterization bits for R channel (1 to 8):");
                        scanf("%hhu", &rbits);
                        printf("Enter the number of posterization bits for G channel (1 to 8):");
                        scanf("%hhu", &gbits);
                        printf("Enter the number of posterization bits for B channel (1 to 8):");
                        scanf("%hhu", &bbits);
                        Posterize(image, rbits, gbits, bbits);
                        printf("\"Posterize\" operation is done!\n");
                        break;
					case 13:
						 printf("Enter the angle of rotation:");
                        scanf("%lf", &Angle);
                        printf("Enter the scale of zooming:");
                        scanf("%lf", &ScaleFactor);
                        printf("Enter the X-axis coordinate of the center of rotation:");
                        scanf("%d", &CenterX);
                        printf("Enter the Y-axis coordinate of the center of rotation:");
                        scanf("%d", &CenterY);
                        Rotate(image, Angle, ScaleFactor, CenterX, CenterY);
                        printf("\"Rotate\" operation is done!\n");
                        break;
					case 14:
						printf("Please input blur amount: ");
						scanf("%d", &BlurAmount);
						MotionBlur(image, BlurAmount);
						printf("\"Blur\" operation is done!\n");
						break;
					case 15:
						printf("Please enter the X offset value: ");
						scanf("%d", &x);
						printf("Please enter the Y offset value: ");
						scanf("%d", &y);
						printf("Please enter the crop width: ");
						scanf("%d", &W);
						printf("Please enter the crop height: ");
						scanf("%d", &H);
						image = Crop(image, x, y, W, H); 
						printf("\"Crop\" operation is done!\n");
						break;
					case 16:
						printf("Please input the new image width: ");
						scanf("%d", &newWidth);
						printf("Please input the new image height: ");
						scanf("%d", &newHeight);
						image = Resize(image, newWidth, newHeight);
						printf("\"Resize\" operation is done!\n");
						break;
					case 17:
						watermark_image = LoadImage("watermark_template");
						if(watermark_image)
						{
							Watermark(image, watermark_image);
							printf("\"Watermark\" operation is done!\n");
							DeleteImage(watermark_image);
						}
						else
						{
							printf("watermark could not be loaded");
						}
						break;
					default:
						break;
				}
			}
		}
		else if (option == 19) {
			r24 = AutoTest();
			printf("AutoTest finished successfully.\n");
			
		}
		else {
			printf("Invalid selection!\n");
		}

		/* Process finished, waiting for another input */
		PrintMenu();
		printf("Please make your choice: ");
		scanf("%d", &option);
	}
	if(image)
	{
		DeleteImage(image);
	}

	printf("You exit the program.\n");
	#endif
	return 0;
}

void PrintMenu(){
	printf("\n----------------------------\n");
	printf(" 1: Load a PPM image\n");
	printf(" 2: Save an image in PPM and JPEG format\n");
	printf(" 3: Change a color image to Black & White\n");
	printf(" 4: Make a negative of an image\n");
	printf(" 5: Color filter an image\n");
	printf(" 6: Sketch the edge of an image\n");
	printf(" 7: Shuffle an image\n");
	printf(" 8: Flip an image vertically\n");
	printf(" 9: Mirror an image horizontally\n");
	printf("10: Add border to an image\n");
	printf("11: Create a fisheye image\n");
	printf("12: Posterize an image\n");
	printf("13: Rotate and zoom an image\n");
	printf("14: Add Motion Blur to an image\n");
	printf("15: Crop an image\n");
	printf("16: Resize an image\n");
	printf("17: Add Watermark to an image\n");
	printf("18: Rotate an image by 90 degrees\n");
	printf("19: Test all functions\n");
	printf("20: Exit\n");
}
