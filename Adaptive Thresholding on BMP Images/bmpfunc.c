#include "bmpfunc.h"
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#ifndef FUNC_GIVEN

int RGB2Gray(unsigned char red, unsigned char green, unsigned char blue){
	// this is a commonly used formula
	double gray = 0.2989 * red + 0.5870 * green + 0.1140 * blue;
	return (int) gray;
}

#endif


#ifndef IMG_TO_GRAY

BMPImage * ImgToGray(BMPImage * image){
  BMPImage * gray_image = malloc(sizeof(BMPImage));
  if(gray_image == NULL){
    return NULL;
  }
  BMPHeader hdr = image->header;
  gray_image->header = hdr;
  gray_image->data = malloc(sizeof(unsigned char)*(image->header).imagesize);
  if(gray_image->data == NULL){
    return NULL;
  }
  int width = image->header.width;
  int height = image->header.height;
  char ** twodg = malloc(sizeof(char*)*height);
  int row;
  int col;
  for(row = 0; row<height;row++){
    twodg[row] = malloc(sizeof(char*)*width);
  }
  int pxl = 0;
  for(row= 0; row<height; row++){
    for(col = 0; col<width; col++){
      twodg[row][col] = RGB2Gray(image->data[pxl+2], image->data[pxl+1], image->data[pxl]);
      pxl+=3;
    }
  }
  pxl = 0;
  for(row = 0; row<height; row++){
    for(col = 0; col<width; col++){
      gray_image->data[pxl+2] = twodg[row][col];
      gray_image->data[pxl+1] = twodg[row][col];
      gray_image->data[pxl] = twodg[row][col];
      pxl+=3;
    }
  }
  for(row = 0; row<height; row++){
    free(twodg[row]);
  }
  free(twodg);
    

	// allocate space for the image
	// the image has the same size
	// therefore the header has to stay the same

  return gray_image;

}

#endif


#ifndef ADAPTIVE_THRESHOLDING
/*double calcthrs(int pn, int rad, int eps, BMPImage * gray, int currpix);
double calcthrs(int pn, int rad, int eps, BMPImage * gray, int cx){
  double thrs = 0;
  double sum = 0;
  double t = 0; //total
  int pxl = cx;
  int i;
  int k;
  int j;
  int countup=0;
  int countdown = 0;
  int width = gray->header.width;
  int height = gray->header.height;
  int curline = cx/pn;
  if(curline == 0){
    curline = 1;
  } 
  else if(pn%cx != 0){
    curline+=1;
  }
  for(j = curline; j > curline-rad; j--){
    if(j > 0){
      countup+=1;
    }
  }
  for(j = curline; j<curline+rad; j++){
    if(j<=height){
      countdown+=1;
    }
  }
  curline = curline + countdown;
  pxl = pxl+width*countdown;
  cx  = cx+width*countdown;
  
  for(k = curline ; k>=countup+countdown; k--){
    for(i = cx; i>=cx - rad; i--){
      if(i>=0){
	sum+=gray->data[pxl];
	t++;
      }
      pxl-=3;
    }
    pxl = cx+3;
    for(i = cx; i<=cx + rad; i++){
      if(i<=pn){
	sum+=gray->data[pxl];
	t++;
      }
      pxl+=3;
    }
    cx -= width-1;
    pxl-= width-1;
  }
  thrs = (sum / t) - eps;
  return thrs;
  }*/
double calcthrs(int rad, int eps, int ** data, int currow, int curcol, int height, int width);
double calcthrs(int rad, int eps, int ** data, int currow, int curcol, int height, int width){
  int i;
  int k;
  double sum = 0;
  int total = 0;
  double thrs = 0;
  for(i = currow; i<=currow+rad; i++){
    for(k = curcol; k<=curcol+rad; k++){
      if(i<height && k<width){
	sum +=data[i][k];
	total+=1;
      }
    }
    for(k = curcol-1; k>=curcol-rad; k--){
      if(i<height && k >= 0){
	sum+=data[i][k];
	total+=1;
      }
    }
  }
  for(i = currow-1; i >= currow-rad; i--){
    for(k = curcol; k<=curcol+rad; k++){
      if(i>=0 && k < width){
	sum+=data[i][k];
	total+=1;
      }
    }
    for(k = curcol-1; k>=curcol-rad; k--){
      if(i>=0 && k>=0){
	sum+=data[i][k];
	total+=1;
      }
    }
  }
  thrs = (sum / total) - eps;
  return thrs;
    
}
BMPImage * AdaptiveThresholding(BMPImage * grayImage, int radius, int epsilon){
  BMPImage * adaptive = NULL;
  adaptive = malloc(sizeof(BMPImage));
  BMPHeader hdr = grayImage->header;
  adaptive->header = hdr;
  adaptive->data = malloc(sizeof(unsigned char)*(grayImage->header).imagesize);
  int height = grayImage->header.height;
  int width = grayImage->header.width;
  int ** twodg = malloc(sizeof(int*)*height);
  int row;
  int col;
  for(row = 0; row<height; row++){
    twodg[row] = malloc(sizeof(int)*width);
  }
  int pxl = 0;
  for(row=0; row<height; row++){
    for(col = 0; col<width; col++){
      twodg[row][col] = grayImage->data[pxl];
      pxl+=3;
    }
  }
  double thrs = 0;
  double curIntensity = 0;
  pxl = 0;
  for(row = 0; row<height; row++){
    for(col = 0; col<width; col++){
      curIntensity = twodg[row][col];
      thrs = calcthrs(radius, epsilon, twodg, row, col, height, width);
      //printf("%f ", thrs);
       if(curIntensity >= thrs){
	adaptive->data[pxl+2] = 255;
	adaptive->data[pxl+1] = 255;
	adaptive->data[pxl] = 255;
      }
      else{
	adaptive->data[pxl+2] = 0;
	adaptive->data[pxl+1] = 0;
	adaptive->data[pxl] = 0;
      }
      pxl+=3;
    }
  }
  for(row = 0; row<height; row++){
    free(twodg[row]);
  }
  free(twodg);
      
      
  //int pixNUM  = grayImage->header.width * grayImage->header.height;
  //printf("%d\n", pixNUM);
  //int curpxl =0;
  
  /*for(int i = 1; i<=pixNUM; i++){
    thrs = calcthrs(pixNUM, radius, epsilon, grayImage, i);
    curIntensity = grayImage->data[curpxl];
    if(curIntensity < 0){
	curIntensity = -curIntensity;
	}
      if(curIntensity >= thrs){
	adaptive->data[curpxl+2] = 255;
	adaptive->data[curpxl+1] = 255;
	adaptive->data[curpxl] = 255;
      }
      else{
	adaptive->data[curpxl+2] = 0;
	adaptive->data[curpxl+1] = 0;
	adaptive->data[curpxl] = 0;
      }
      curpxl+=3;
      }*/
 	// allocate space for the image
	// the image has the same size
	// therefore the header has to stay the same

  return adaptive;

}


#endif
