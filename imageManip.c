/* Brayden Archer and Sam Lipschitz
 * barcher9 and slipsch3
 * imageManip.c
 * 601.220, Spring 2019
 */

#include <assert.h>
#include "imageManip.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

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
  }else{//if rows/cols don't match for both images
    //creating 2D Pixel arrays for both images
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
	  }
	}
      }
    }

  }
  //setting im->data to pix2d
  for(int i = 0; i < im->rows; i++){
    for(int j = 0; j < im->cols; j++){
      new->data[(i * im->cols) + j] = pix2d[i][j];
    }
  }
  //freeing 2d array
  for(int i = 0; i < im->rows; i++){
    free(pix2d[i]);
  }
  free(pix2d);
  return new;
}

/* Returns blurred version of given Image.
 * When sigma is larger, the output will have a stronger blurred effect.
 */
Image* blur(const Image * im, double sigma){
  Image * new = malloc(sizeof(Image));
  new->rows = im->rows;
  new->cols = im->cols;
  new->data = malloc(sizeof(Pixel) * new->rows * new->cols); 
  //creating 2D pixel array
  Pixel ** pix2d;
  pix2d = malloc(sizeof(*pix2d) * im->rows);
  for(int i = 0; i < im->rows; i++){                                                               
    pix2d[i] = malloc(im->cols * sizeof(pix2d[0]));
  }
  for(int i = 0; i < im->rows; i++){
    for(int j = 0; j < im->cols; j++){
      Pixel * temp = filterResponse(sigma, im, i, j);
      pix2d[i][j] = *temp;
      free(temp);
    }
  }
  //setting new->data to 2D array
  for(int i = 0; i < im->rows; i++){
    for(int j = 0; j < im->cols; j++){
      new->data[(i * im->cols) + j] = pix2d[i][j];
    }
  }
  for(int i = 0; i < im->rows; i++){
    free(pix2d[i]);
  }
  free(pix2d);
  return new;
}

/* Returns the square of the given double. */
double sq(double p){
  return p * p;
}

/* Returns a 2D double Gaussian matrix with  variance sigma. */
double ** createGM(double sigma){
  int n = 10 * sigma;
  if((n % 2) == 0){//if n is even
    n += 1;
  }
  //don't forget to free everything eventually when this method is called
  double ** gm = malloc(n * sizeof(double));
  for(int i = 0; i < n; i++){
    gm[i] = malloc(n * sizeof(double));
  }
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      double dy = abs(n / 2 - i);
      double dx = abs(n / 2 - j);
      gm[i][j] = (1.0 / (2.0 * PI * sq(sigma))) * exp( -(sq(dx) + sq(dy)) / (2 * sq(sigma)));
    }
  }
  return gm;
}

/* Returns filter response using filter gm for a Pixel in im with
 * given row and col.
 */
Pixel* filterResponse(double sigma, const Image * im, int row, int col){
  //printf("In filter response, row %d col %d \n", row, col);
  int n = 10 * sigma;
  //printf("%d\n", n);
  if((n % 2) == 0){//if n is even
    n += 1;
  }
  //making a filter for each color channel
  double ** filter_r = createGM(sigma);
  double ** filter_g = createGM(sigma);
  double ** filter_b = createGM(sigma);
  double ** filter_temp = createGM(sigma);
  //calculating initial sum of values in Gaussian matrix
  double sum_gm = 0;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      sum_gm += filter_r[i][j];
    }
  }
  Pixel *output = malloc(sizeof(Pixel));
  //setting pixel at given row/col at center of matrix and multiplying each matrix element by
  //pixel value beneath it
  for(int i = - n / 2; i < n / 2 + 1; i++){
    for(int j = - n / 2; j < n/2 + 1; j++){
      //if a Pixel exists below
      //if(!((row < n/2) || (col < n/2) || (row >= im->rows -(n/2)) || (col >= im->cols - (n/2)))){
      if((i + row >= 0) && (j + col >= 0) && (i+ row < im->rows ) && (j + col < im->cols)){
	//printf("no skip, i=%d, j= %d\n", i, j);
	filter_r[i + (n / 2)][j + (n / 2)] *= im->data[(row + i ) * im->cols + col + j].r;
	//printf(" (pass)\n");
	filter_g[i + (n / 2)][j + (n / 2)] *= im->data[(row + i ) * im->cols + col + j].g;
	filter_b[i + (n / 2)][j + (n / 2)] *= im->data[(row + i ) * im->cols + col + j].b;
      }else{//if Pixel doesn't exist below, set to 0
	//printf("skip, i=%d, j= %d\n", i, j);
	filter_r[i + (n / 2)][j + (n / 2)] = 0;
	filter_g[i + (n / 2)][j + (n / 2)] = 0;
	filter_b[i + (n / 2)][j + (n / 2)] = 0;
	sum_gm -= filter_temp[i + (n / 2)][j + (n / 2)];
      }
    }
  }
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      //printf("%f ", filter_r[i][j]);
    }
    //printf("\n");
  }
  //calculating sum of weighted Pixel values for each color channel
  double sum_r = 0;
  double sum_g = 0;
  double sum_b = 0;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      sum_r += filter_r[i][j];
      sum_g += filter_g[i][j];
      sum_b += filter_b[i][j];
    }
  }
  //setting output Pixel's color channels to normalized sum of values in filter matrix
  output->r = (unsigned char)(sum_r / sum_gm);
  output->g = (unsigned char)(sum_g / sum_gm);
  output->b = (unsigned char)(sum_b / sum_gm);

  //freeing each color channel filter
  for(int i = 0; i < n; i++){
    free(filter_r[i]);
    free(filter_g[i]);
    free(filter_b[i]);
    free(filter_temp[i]);
  }
  free(filter_r);
  free(filter_g);
  free(filter_b);
  free(filter_temp);
  return output;
}

Image * zoom_in(Image * input1) {
  //TODO: Create an input 2D array from input1 and also create a 2D output array that has double the rows and cols of input1
  Image * input = malloc(sizeof(Image));
  input->rows = input1->rows;
  input->cols = input1->cols;
  input->data = malloc(sizeof(Pixel) * input->rows * input->cols);
  
  Pixel ** output = malloc(sizeof(*output) * 2 * input1->rows); 

  for (int i = 0; i < output->rows; i++) {
    output[i] = malloc(input1->cols * 2 * sizeof(output[0]));
  }
  


  //Variables to keep track of the current row and col for input image
  int row = 0;
  int col = 0;
  //For loop only has to keep track of output image dimensions because its guaranteed to be double the input dimensions. Row and col interate once for every two iterations of output because of += 2 
  for (int i = 0; i < output->rows; i += 2) {
    for (int j = 0; j < output->cols; j += 2) {
      output->data[i][j].r = input->data[row][col].r;
      output->data[i][j].g = input->data[row][col].g;
      output->data[i][j].b = input->data[row][col].b;

      output->data[i+1][j+1].r = input->data[row][col].r;
      output->data[i+1][j+1].g = input->data[row][col].g;
      output->data[i+1][j+1].b = input->data[row][col].b;

      output->data[i+1][j].r = input->data[row][col].r;
      output->data[i+1][j].g = input->data[row][col].g;
      output->data[i+1][j].b = input->data[row][col].b;

      output->data[i][j+1].r = input->data[row][col].r;
      output->data[i][j+1].g = input->data[row][col].g;
      output->data[i][j+1].b = input->data[row][col].b;
      col++;
    }
    row++;
  }
  return output;
}

Image * zoom_out(Image * input1) {
  return input1;
}

int doOperation(char *argv[]){
  FILE * inputF = fopen(argv[1], "rb");
  Image * inputI = read_ppm(inputF);
  FILE * outputF = fopen(argv[2], "wb");
  Image * outputI = NULL;

  if(strcmp(argv[3], "exposure") == 0){
    double ev;
    sscanf(argv[4], "%lf", &ev);
    outputI = exposure(inputI,ev);
  }
  
  if(strcmp(argv[3], "blend") == 0){
    double alpha;
    sscanf(argv[5], "%lf", &alpha);
    FILE * inputF2 = fopen(argv[4], "rb");
    Image * inputI2 = read_ppm(inputF2);
    outputI = blend(inputI, inputI2, alpha);
    fclose(inputF2);
    free(inputI2->data);
    free(inputI2);
  }

  if(strcmp(argv[3], "zoom_in") == 0){
    outputI = inputI;
  }

  if(strcmp(argv[3], "zoom_out") == 0){
    outputI = inputI;
  }
  
  if(strcmp(argv[3], "pointilism") == 0){
    outputI = pointilism(inputI);
  }

  if(strcmp(argv[3], "swirl") == 0){
    outputI = inputI;
  }

  if(strcmp(argv[3], "blur") == 0){
    double sigma;
    sscanf(argv[4], "%lf", &sigma);
    outputI = blur(inputI, sigma);
  }
  
  if(write_ppm(outputF, outputI) == -1){
    printf("Error: unable to open specified output file for writing or writing output failed.\n");
    fclose(inputF);
    fclose(outputF);
    free(inputI->data);
    free(inputI);
    free(outputI->data);
    free(outputI);
    return 7;
  }
  fclose(inputF);
  fclose(outputF);
  free(inputI->data);
  free(inputI);
  free(outputI->data);
  free(outputI);
  return 0;
}
