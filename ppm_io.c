/* Brayden Archer and Sam Lipschitz
 * barcher9 and slipsch3
 * ppm_io.c
 * 601.220, Spring 2019
 */

#include <assert.h>
#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check that fp is not NULL
  //assert(fp);
  
  //check that file is PPM file
  char buf[3];
  for(int i = 0; i < 3; i++){
    buf[i] = getc(fp);
  }
  //assert(buf[0] == 'P' && buf[1] == '6' && isspace(buf[2]));
  if(!(buf[0] == 'P' && buf[1] == '6' && isspace(buf[2]))){
    return NULL;
  }
  
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
  if(w < 0 || h < 0){
    return NULL;
  }
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
  //assert(fp); 
  if(fp == NULL){
    return -1;
  }
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

int error_report(int argc, char *argv[]){
  if(argc < 3){
    printf("Error: must specify input file name and output file name.\n");
    return 1;
  }
  FILE * inputF = fopen(argv[1], "rb");
  if(inputF == NULL){
    printf("Error: could not open specified input file.\n");                            
    return 2;
  }
  
  Image * inputI = read_ppm(inputF);
  if(inputI == NULL){
    fclose(inputF);
    printf("Error: failed to read input file.  Must be properly-formatted PPM file.\n");
    return 3;
  }
  int validOperation = 1;
  if(strcmp(argv[3], "blur") == 0 || strcmp(argv[3], "swirl") == 0 || strcmp(argv[3], "pointilism") == 0 || strcmp(argv[3], "zoom_out") == 0 || strcmp(argv[3], "zoom_in") == 0 || strcmp(argv[3], "blend") == 0 || strcmp(argv[3], "exposure") == 0){
    validOperation = 0;
  }
  if(validOperation != 0){
    printf("Error: must include valid operation name.\n");
    fclose(inputF);
    free(inputI->data);
    free(inputI);
    return 4;
  }
  if(strcmp(argv[3], "zoom_in") == 0 || strcmp(argv[3], "zoom_out") == 0 || strcmp(argv[3], "pointilism") == 0){
    if(argc != 4){
      printf("Incorrect number of arguments for specified operation.\n");
      fclose(inputF);
      free(inputI->data);
      free(inputI);
      return 5;
    }
  }
  if(strcmp(argv[3], "exposure") == 0 || strcmp(argv[3], "blur") == 0){
    if(argc != 5){
      printf("Incorrect number of arguments for specified operation.\n");
      fclose(inputF);
      free(inputI->data);
      free(inputI);
      return 5;
    }
    if(strcmp(argv[3], "exposure") == 0){
      double arg;
      int count = sscanf(argv[4], "%lf", &arg);
      if(count != 1 || arg < -3 || arg > 3){
        printf("Invalid argument(s) for specified operation.\n");
        fclose(inputF);
        free(inputI->data);
        free(inputI);
        return 6;
      }
    }else{
      double arg;
      int count = sscanf(argv[4], "%lf", &arg);
      if(count != 1 || arg <= 0){
        printf("Invalid argument(s) for specified operation.\n");
        fclose(inputF);
        free(inputI->data);
        free(inputI);
        return 6;
      }
    }
  }
  if(strcmp(argv[3], "swirl") == 0){
    if(argc != 7){
      printf("Incorrect number of arguments for specified operation.\n");
      fclose(inputF);
      free(inputI->data);
      free(inputI);
      return 5;
    }
    int col;
    int row;
    int scale;
    int count = 0;
    count += sscanf(argv[4], "%d", &col);
    count += sscanf(argv[5], "%d", &row);
    count += sscanf(argv[6], "%d", &scale);
    if(count != 3 || col < 0 || col > inputI->cols - 1 || row < 0 || row > inputI->rows - 1 || scale <= 0){
      printf("Invalid argument(s) for specified operation.\n");
      fclose(inputF);
      free(inputI->data);
      free(inputI);
      return 6;
    }
  }
  if(strcmp(argv[3], "blend") == 0){
    if(argc != 6){
      printf("Incorrect number of arguments for specified operation.\n");
      fclose(inputF);
      free(inputI->data);
      free(inputI);
      return 5;
    }
    double arg;
    int count = sscanf(argv[5], "%lf", &arg);
    if(arg <= 0 || arg >= 1 || count != 1){
      printf("Invalid argument(s) for specified operation.\n");
      fclose(inputF);
      free(inputI->data);
      free(inputI);
      return 6;
    }
    FILE * input2_f = fopen(argv[4], "rb");
    if(input2_f == NULL){
      printf("Error: could not open second file.\n");
      fclose(inputF);
      free(inputI->data);
      free(inputI);
      return 2;
    }
    Image * input2_i = read_ppm(input2_f);
    if(input2_i == NULL){
      printf("Error: failed to read second file.  Must be properly-formatted PPM file.\n");
      fclose(inputF);
      free(inputI->data);
      free(inputI);
      fclose(input2_f);
      return 3;
    }
    fclose(input2_f);
    free(input2_i->data);
    free(input2_i);
  }
  fclose(inputF);
  free(inputI->data);
  free(inputI);
  return 0;
}
