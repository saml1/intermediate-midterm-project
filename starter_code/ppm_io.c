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
  //printf("%c%c%c\n", buf[0], buf[1], buf[2]);
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

  //Pixel pixels[w*h];
  Pixel *pixels = malloc(sizeof(Pixel)*w*h);
  
  fread(pixels, sizeof(Pixel), w*h, fp);
  //printf("%d\n", (int)pixels[3].r);
  
  
  Image * im = malloc(sizeof(Image));
  //printf("ss\n");
  im -> data = pixels;
  im -> cols = w;
  im -> rows = h; 
  
  return im;  //TO DO: replace this stub
  
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

Image * exposure(Image * orig, float ev){
  Image * new = malloc(sizeof(Image));;
  //printf("%d\n", orig->data[329].r);
  Pixel *pixels = malloc(sizeof(Pixel)*orig->rows*orig->cols);
  new->data = pixels;
  new->rows = orig->rows;
  new->cols = orig->cols;
  for(int i = 0; i < (orig->rows)*(orig->cols); i++){
    //printf("%d\n", orig->data[321].r);
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
    /*new->data[i].g = orig->data[i].g * pow(2, ev);
      new->data[i].b = orig->data[i].b * pow(2, ev);*/
    /*if(new->data[i].r >= 255){
      new->data[i].r = 255;
    }
    if(new->data[i].g >= 255){
      new->data[i].g = 255;
    }
    if(new->data[i].b >= 255){
      new->data[i].b = 255;
      }*/
  }
  //printf("%d\n", orig->data[329].r);
  return new;
}

Image * blend(Image * input1, Image * input2, float alpha) {
  Image * new = malloc(sizeof(Image)); //Allocate memory for the new outut Image

  //finding max value of rows and cols
  int rows = 0;
  int cols = 0;
  rows = fmax(input1->rows, input2->rows);
  cols = fmax(input1->cols, input2->cols);
  printf("%d %d\n", rows, cols);
  //Pixel * pixels = (Pixel *)malloc(sizeof(Pixel) * (rows) * (cols));
  new->data = (Pixel *)malloc(sizeof(Pixel) * (rows) * (cols));
  int col1 = 0;
  int row1 = 0;
  int col2 = 0;
  int row2 = 0;
  //new->data = pixels;
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
    Pixel pix1[input1->rows][input1->cols];
    Pixel pix2[input2->rows][input2->cols];
    Pixel newPix2d[(int)fmax(input1->rows, input2->rows)][(int)fmax(input1->cols, input2->cols)];
    
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
    //for(int i = fmin(input1->cols, input2->cols)
    //setting 1D pixel array of output to 2D array
    for(int i = 0; i < rows; i++){
      for(int j = 0; j < cols; j++){
	//pixels[(i * cols) + j] = newPix2d[i][j];
	new->data[(i * cols) + j] = newPix2d[i][j];
      }
    }
  }
  return new;
}
  
