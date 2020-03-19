//Brayden Archer and Sam Lipschitz
//barcher9 and slipsch3
// ppm_io.c
// 601.220, Spring 2019
// Starter code for midterm project - feel free to edit/add to this file

#include <assert.h>
#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check that fp is not NULL
  assert(fp);
  
  //check that file is PPM file
  char buf[3];
  for(int i = 0; i < 3; i++){
    buf[i] = getc(fp);
  }
  assert(buf[0] == 'P' && buf[1] == '6' && isspace(buf[2]));

  //ignoring comment line
  char c = getc(fp);
  if(c == '#'){
    while(c != '\n'){
      c = getc(fp);
    }
  }
  ungetc(c, fp);//puts c back into input stream so w/h are read properly
  
  //reading width/height of image
  int w = 0;
  int h = 0;
  fscanf(fp, "%d %d", &w, &h);
  
  //making sure maxval is 255
  int maxval = 0;
  fscanf(fp, "%d", &maxval);
  if(maxval != 255){
    printf("Maxval != 255!\n");
    return NULL;
  }
  c=getc(fp);

  Pixel *pixels = malloc(sizeof(Pixel)*w*h);
  
  fread(pixels, sizeof(Pixel), w*h, fp);
  Image * im = malloc(sizeof(Image));
  im -> data = pixels;
  im -> cols = w;
  im -> rows = h; 
  
  return im;
  
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  assert(fp); 

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->cols * im->rows, fp);

  if (num_pixels_written != im->cols * im->rows) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
  }

  return num_pixels_written;
}

/* Takes Image * fp and returns a new Image * with modified exposure ev. */
Image * exposure(const Image * orig, float ev){
  Image * new = malloc(sizeof(Image));;
  Pixel *pixels = malloc(sizeof(Pixel)*orig->rows*orig->cols);
  new->data = pixels;
  new->rows = orig->rows;
  new->cols = orig->cols;
  for(int i = 0; i < (orig->rows)*(orig->cols); i++){
    if(orig->data[i].r * pow(2, ev) > 255){
      new->data[i].r = 255;
    }else{
      new->data[i].r = orig->data[i].r * pow(2, ev);
    }
    if(orig->data[i].g * pow(2, ev) > 255){
      new->data[i].g = 255;
    }else{
      new->data[i].g = orig->data[i].g * pow(2, ev);
    }
    if(orig->data[i].b * pow(2, ev) > 255){
      new->data[i].b = 255;
    }else{
      new->data[i].b = orig->data[i].b * pow(2, ev);
    }
  }
  return new;
}

/* Takes two Image *'s and returns a blended image of the two with
 * ratio alpha.
 */
Image * blend(const Image * input1, const Image * input2, float alpha) {
  Image * new = malloc(sizeof(Image)); //Allocate memory for the new outut Image

  //finding max value of rows and cols
  int rows = 0;
  int cols = 0;
  rows = fmax(input1->rows, input2->rows);
  cols = fmax(input1->cols, input2->cols);
  new->data = (Pixel *)malloc(sizeof(Pixel) * (rows) * (cols));
  int col1 = 0;
  int row1 = 0;
  int col2 = 0;
  int row2 = 0;
  new->rows = rows;
  new->cols = cols;
  int sameDims = 0;
  if((input1->rows == input2->rows) && (input1->cols == input2->cols)){
    sameDims = 1;
  }
  //looping through each pixel in new image
  if(sameDims){
    for(int i = 0; i < rows * cols; i++){
    
      //if the rows/cols match up for both images simply use given calculation
      if((row1==row2) && (col1 == col2)){
	if((alpha*(input1->data[i].r) + (1-alpha)*(input2->data[i].r)) > 255){
	  new->data[i].r = 255;
	}else{
	  new->data[i].r = alpha*input1->data[i].r + (1-alpha)*input2->data[i].r;
	}
	if(alpha*input1->data[i].g + (1-alpha)*input2->data[i].g > 255){
	  new->data[i].g = 255;
	}else{
	  new->data[i].g = alpha*input1->data[i].g + (1-alpha)*input2->data[i].g;
	}
	if(alpha*input1->data[i].b + (1-alpha)*input2->data[i].b > 255){
	  new->data[i].b = 255;
	}else{
	  new->data[i].b = alpha*input1->data[i].b + (1-alpha)*input2->data[i].b;
	}
	//finding "coordinate" of pixel from each input
	if((i+1) % input1->cols == 0){
	  row1 += 1;
	  col1 = 0;
	}else{
	  col1+=1;
	}
	if((i+1) % input2->cols == 0){
	  row2 += 1;
	  col2 = 0;
	}else{
	  col2+=1;
	}
      } 
    }
  }else{
    Pixel ** pix1;
    pix1 = malloc(input1->rows * sizeof(*pix1));
    for(int i = 0; i < input1->rows; i++){
      pix1[i] = malloc(input1->cols * sizeof(pix1[0]));
    }
    Pixel ** pix2;
    pix2 = malloc(input2->rows * sizeof(*pix2));
    for(int i = 0; i < input2->rows; i++){
      pix2[i] = malloc(input2->cols * sizeof(pix2[0]));
    }
    Pixel ** newPix2d;
    newPix2d = malloc(rows * sizeof(*newPix2d));
    for(int i = 0; i < rows; i++){
      newPix2d[i] = malloc(cols * sizeof(newPix2d[0]));
    }
    //allocating all pixels initially to 0/0/0
    for(int i = 0; i < rows; i++){
      for(int j = 0; j < cols; j++){
	newPix2d[i][j].r = 0;
	newPix2d[i][j].g = 0;
	newPix2d[i][j].b = 0;
      }
    }
    //converting both 1D pixel arrays into 2D
    for(int i = 0; i < input1->rows; i++){
      for(int j = 0; j < input1->cols; j++){
	pix1[i][j] = input1->data[(i*input1->cols) + j];
      }
    }
    for(int i = 0; i < input2->rows; i++){
      for(int j = 0; j < input2->cols; j++){
	pix2[i][j] = input2->data[(i*input2->cols) + j];
      }
    }
    //blending all possible pixels 
    for(int i = 0; i < fmin(input1->rows, input2->rows); i++){
      for(int j = 0; j < fmin(input1->cols, input2->cols); j++){
	if((alpha*pix1[i][j].r + (1 - alpha)*pix2[i][j].r) > 255){
	  newPix2d[i][j].r = 255;
	}else{
	  newPix2d[i][j].r = alpha*pix1[i][j].r + (1 - alpha)*pix2[i][j].r;
	}
	if((alpha*pix1[i][j].g + (1 - alpha)*pix2[i][j].g) > 255){
          newPix2d[i][j].g = 255;
	}else{
          newPix2d[i][j].g = alpha*pix1[i][j].g + (1 - alpha)*pix2[i][j].g;
	}
	if((alpha*pix1[i][j].b + (1 - alpha)*pix2[i][j].b) > 255){
          newPix2d[i][j].b = 255;
	}else{
          newPix2d[i][j].b = alpha*pix1[i][j].b + (1 - alpha)*pix2[i][j].b;
	}
      }
    }
    //finding which image has greater amount of rows and cols
    int bigColIs1 = 0;
    int bigRowIs1 = 0;
    //setting to -1 if row or col is equal 
    if(input1->rows == input2->rows){
      bigRowIs1 = -1;
    }
    if(input1->cols == input2->cols){
      bigColIs1 = -1;
    }
    if(input1->rows > input2->rows){
      bigRowIs1 = 1;
    }
    if(input1->cols > input2->cols){
      bigColIs1 = 1;
    }
    //setting color channels of non-overlapping rows to that of image with greater rows
    for(int i = fmin(input1->rows, input2->rows); i < rows; i++){
      if(bigRowIs1 == 1){
	for(int j = 0; j < input1->cols; j++){
	  newPix2d[i][j].r = pix1[i][j].r;
	  newPix2d[i][j].g = pix1[i][j].g;
	  newPix2d[i][j].b  = pix1[i][j].b;
	}
      }else if(bigRowIs1 == 0){
	for(int j = 0; j < input2->cols; j++){
          newPix2d[i][j].r = pix2[i][j].r;
          newPix2d[i][j].g = pix2[i][j].g;
          newPix2d[i][j].b  = pix2[i][j].b;
	}
      }
    }
    if(bigColIs1 == 1){
      for(int i = 0; i < input1->rows; i++){
	for(int j = fmin(input1->cols, input2->cols); j < cols; j++){
	  newPix2d[i][j].r = pix1[i][j].r;
          newPix2d[i][j].g = pix1[i][j].g;
          newPix2d[i][j].b  = pix1[i][j].b;
	}
      }
    }
    if(bigColIs1 == 0){
      for(int i = 0; i < input2->rows; i++){
	for(int j = fmin(input1->cols, input2->cols); j < cols; j++){
          newPix2d[i][j].r = pix2[i][j].r;
          newPix2d[i][j].g = pix2[i][j].g;
          newPix2d[i][j].b  = pix2[i][j].b;
	}
      } 
    }
    //setting 1D pixel array of output to 2D array
    for(int i = 0; i < rows; i++){
      for(int j = 0; j < cols; j++){
	new->data[(i * cols) + j] = newPix2d[i][j];
      }
    }
    for(int i = 0; i < rows; i++){
      free(newPix2d[i]);
    }
    free(newPix2d);
    for(int i = 0; i < input1->rows; i++){
      free(pix1[i]);
    }
    free(pix1);
    for(int i = 0; i < input2->rows; i++){
      free(pix2[i]);
    }
    free(pix2);
  }
  return new;
}

/* Takes image and returns new Image * with a pointilism-like effect */
Image * pointilism(const Image * im){
  int numPix = im->rows * im->cols;
  Image * new = malloc(sizeof(Image));
  new->rows = im->rows;
  new->cols = im->cols;
  new->data = malloc(sizeof(Pixel)* numPix);
  //converting to 2d array
  Pixel ** pix2d;
  pix2d = malloc(im->rows * sizeof(*pix2d));
  for(int i = 0; i < im->rows; i++){
    pix2d[i] = malloc(im->cols * sizeof(pix2d[0]));
  }
  //randomly picking 3% of pixels and applying pointilism effect
  int curx = 0;
  int cury = 0;
  int curr = 0;
  for(int i = 0; i < im->rows; i++){
    for(int j = 0; j < im->cols; j++){
      pix2d[i][j]= im->data[(i * im->cols) + j];
      //pix2d[i][j].g = 0;
      //pix2d[i][j].b = 0;
    }
  }
  for(int i = 0; i < numPix * 0.03; i++){
    curx = rand() % im->cols;
    cury = rand() % im->rows;
    curr = rand() % 5 + 1;
    //starting at pixel curr up and curr to the left
    for(int i = cury - curr; i < cury + curr + 1; i++){
      for(int j = curx - curr; j < curx + curr + 1; j++){
	//if pixel exists
	if((i >= 0) && (i < im->rows) && (j >= 0) && (j < im->cols)){
	  //if pixel is within radius (using distance formula) set its color to center of circle
	  if(sqrt(pow((i-cury), 2) + pow((j-curx),2)) < (double)curr){
	    pix2d[i][j].r = pix2d[cury][curx].r;
	    pix2d[i][j].g = pix2d[cury][curx].g;
	    pix2d[i][j].b = pix2d[cury][curx].b;
	    //printf("%d\n", (int)pix2d[i][j].r);
	  }
	}
      }
    }
    /*for(int i = 1; i < curr + 1; i++){
      pix2d[curx][cury + i].r = pix2d[curx][cury].r;
      pix2d[curx][cury + i].g = pix2d[curx][cury].g;
      pix2d[curx][cury + i].b = pix2d[curx][cury].b;

      pix2d[curx][cury - i].r = pix2d[curx][cury].r;
      pix2d[curx][cury - i].g = pix2d[curx][cury].g;
      pix2d[curx][cury - i].b = pix2d[curx][cury].b;

      pix2d[curx + i][cury].r = pix2d[curx][cury].r;
      pix2d[curx + i][cury].g = pix2d[curx][cury].g;
      pix2d[curx + i][cury].b = pix2d[curx][cury].b;

      pix2d[curx - i][cury].r = pix2d[curx][cury].r;
      pix2d[curx - i][cury].g = pix2d[curx][cury].g;
      pix2d[curx - i][cury].b = pix2d[curx][cury].b;
      }*/
  }
  //setting im->data to pix2d
  for(int i = 0; i < im->rows; i++){
    for(int j = 0; j < im->cols; j++){
      new->data[(i * im->cols) + j] = pix2d[i][j];
    }
  }
  //printf("%d\n", (int)new->data[1028].r);
  //printf("%d\n", (int)pix2d[345][542].r);
  //freeing 2d array
  for(int i = 0; i < im->rows; i++){
    free(pix2d[i]);
  }
  free(pix2d);
  return new;
}
