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
    new->data[i].r = orig->data[i].r * pow(2, ev);
    new->data[i].g = orig->data[i].g * pow(2, ev);
    new->data[i].b = orig->data[i].b * pow(2, ev);
    if(new->data[i].r >= 255){
      new->data[i].r = 255;
    }
    if(new->data[i].g >= 255){
      new->data[i].g = 255;
    }
    if(new->data[i].b >= 255){
      new->data[i].b = 255;
    }
  }
  //printf("%d\n", orig->data[329].r);
  return new;
}
