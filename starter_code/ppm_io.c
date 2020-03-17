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
  if(input1->rows > input2->rows){
    rows = input1->rows;
  }else{
    rows = input2->rows;
  }
  if(input1->cols > input2->cols){
    cols = input1->cols;
  }else{
    cols = input2->cols;
  }

  Pixel * pixels = malloc(sizeof(Pixel) * rows * cols);
  int col1 = 0;
  int row1 = 0;
  int col2 = 0;
  int row2 = 0;
  //int cur_row = 0;
  //int cur_col = 0;
  int atSingleRows = 0;
  //Pixel * pixels = malloc(sizeof(Pixel)* rows * cols);
  new->data = pixels;
  new -> rows = rows;
  new-> cols = cols;
  //TODO: add case for row 0/col 0
  //looping through each pixel in new image
  for(int i = 0; i < rows * cols; i++){
    printf("%d ", i);
    //Pixel * pixel1 = malloc(sizeof(Pixel));
    //Pixel * pixel2 = malloc(sizeof(Pixel));
    /*Pixel * pixels = malloc(sizeof(Pixel)* rows * cols);
    new->data = pixels;
    new -> rows = rows;
    new-> cols = cols;*/
    //if the rows/cols match up for both images simply use given calculation
    if((row1==row2) && (col1 == col2)){
      if(alpha*input1->data[i].r + (1-alpha)*input2->data[i].r > 255){
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
      if(i+1 % input1->cols == 0){
	row1 += 1;
	col1 = 0;
      }else{
	col1+=1;
      }
      if(i+1 % input2->cols == 0){
	row2 += 1;
	col2 = 0;
      }else{
	col2+=1;
      }
    }else{
      if(col1 +1 > input2->cols){//if at a col that doesn't exist for input2
	new->data[i].r = input1->data[(row1*input1->cols)+col1].r;
	new->data[i].g = input1->data[(row1*input1->cols)+col1].g;
	new->data[i].b = input1->data[(row1*input1->cols)+col1].b;
	if(i+1 % input1->cols == 0 && row1 +1 < input1->rows){//if at end of a col and another row exists for input1
	  row1 += 1;
	  col1 = 0;
	}else if(row1 +1 < input1->rows){//if not at end of col
	  col1+=1;
	}
	else{//at last row of input1 
	  row2 += 1;
	  col2 = 0;
	  atSingleRows = 1;
	}
      }else if(col1 == 0 && atSingleRows == 0){//if just started new col for input1 (where input1 has bigger col)
	col2 = 0;
	row2 = row1;
      }
      if(atSingleRows == 1 && row2 + 1 < input2->rows){
	new->data[i].r = input2->data[(row2*input2->cols)+col2].r;
	new->data[i].g = input2->data[(row2*input2->cols)+col2].g;
	new->data[i].b = input2->data[(row2*input2->cols)+col2].b;
	if(i+1 % input2->cols == 0){
	  row2 += 1;
	  col2 = 0;
	}else{
	  col2+=1;
	}
      }
      else if(atSingleRows == 1){
	new->data[i].r = 0;
	new->data[i].g = 0;
	new->data[i].b = 0;
	printf("fuck");
      }
    } 
  }
  /*Pixel *in1p = malloc(sizeof(Pixel) * input1->rows * input1->cols);
  Pixel *in2p = malloc(sizeof(Pixel) * input2->rows * input2->cols);
  new->data = in1p; //Setting base pixels as image 1
  new->rows = in1p->rows;
  new->cols = in1p->cols;

  for(int i = 0; i < (in1p->rows) * (in1p->cols); i++) {
    float r = alpha * (in1p->data[i].r) + (1-alpha) * (in2p->data[i].r);
    float g = alpha * (in1p->data[i].g) + (1-alpha) * (in2p->data[i].g);
    float b = alpha * (in1p->data[i].b) + (1-alpha) * (in2p->data[i].b);
    if(r > 255)
      new->data[i].r = 255;
    else
      new->data[i].r = r;
    if (g > 255)
      new->data[i].g = 255;
    else
      new->data[i].g = g;
    if (b > 255)
      new->data[i].b = 255;
    else
      new->data[i].b = b;
    }
  free(in1p);
  free(in2p);*/
  
  return new;
}
  
