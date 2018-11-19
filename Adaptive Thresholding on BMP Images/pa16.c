#include "pa16.h"

#ifndef MAIN_FUNC

int main(int argc, char **argv){
  if(argc != 5){
    return EXIT_FAILURE;
  }
  int radius = atoi(argv[3]);
  int eps = atoi(argv[4]);
  if(radius <= 0 || eps <= 0){
    printf("Wrong inputs\n");
    return EXIT_FAILURE;
  }
  BMPImage * orig = NULL;
  orig = BMP_Open(argv[1]);
  if(orig == NULL){
    printf("Error opening BMP file\n");
    return EXIT_FAILURE;
  }
  
  BMPImage * gray_image = NULL;
  gray_image = ImgToGray(orig);
  if(gray_image == NULL){
    printf("Error converting to Gray image\n");
    return EXIT_FAILURE;
  }
  
  BMPImage * adaptive = NULL;
  adaptive = AdaptiveThresholding(gray_image, radius, eps);
  if(adaptive == NULL){
    printf("Error in Adaptive Gray image\n");
    return EXIT_FAILURE;
  }
  if(BMP_Write(argv[2], adaptive) != TRUE){
    printf("Writing failed\n");
    BMP_Free(orig);
    BMP_Free(gray_image);
    BMP_Free(adaptive);
    return EXIT_FAILURE;
  }
  BMP_Free(orig);
  BMP_Free(gray_image);
  BMP_Free(adaptive);
  // check the arguments
  // check radius and epsilon values
  // open the BMP file
  // convert to gray scale
	// check for error in converting to gray scale

  // call adaptive threshold function
  // check for errors after calling adaptive threshold
	// write the adaptive threshold image to file
	// free all the images
  return EXIT_SUCCESS;
}

#endif
