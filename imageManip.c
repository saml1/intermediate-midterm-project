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
    pix2d[i] = malloc(im->cols * sizeof(Pixel));
    //pix2d[i] = malloc(im->cols * sizeof(pix2d[0]));
  }
  //randomly picking 3% of pixels and applying pointilism effect
  int curx = 0;
  int cury = 0;
  int curr = 0;
  for(int i = 0; i < im->rows; i++){
    for(int j = 0; j < im->cols; j++){
      //pix2d[i][j]= im->data[(i * im->cols) + j];
      pix2d[i][j].r= im->data[(i * im->cols) + j].r;
      pix2d[i][j].g= im->data[(i * im->cols) + j].g;
      pix2d[i][j].b= im->data[(i * im->cols) + j].b;
    }
  }
  //printf("numpix: %d\n", numPix);
  for(int p = 0; p < numPix * 0.03; p++){
    int count = 0;
    curx = rand() % im->cols;
    cury = rand() % im->rows;
    curr = rand() % 5 + 1;
    //starting at pixel curr up and curr to the left
    for(int i = cury - curr; i < cury + curr + 1; i++){
      for(int j = curx - curr; j < curx + curr + 1; j++){
	//if pixel exists
	if((i >= 0) && (i < im->rows) && (j >= 0) && (j < im->cols)){
	  //if pixel is within radius (using distance formula) set its color to center of circle

	  if(pow((j - curx), 2) + pow((i - cury),2) <= pow(curr, 2)){
	    //if(sqrt(pow((i-cury), 2) + pow((j-curx),2)) < (double)curr){
	    pix2d[i][j].r = pix2d[cury][curx].r;
	    pix2d[i][j].g = pix2d[cury][curx].g;
	    pix2d[i][j].b = pix2d[cury][curx].b;
	    count += 1;
	  }
	}
      }
    }
    //printf("curx: %d, cury: %d, curr: %d, count: %d\n", curx, cury, curr, count);

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

Image * zoom_in(const Image * input1) {
  //Create an input 2D array from input1 and also create a 2D output array that has double the rows and cols of input1
  int irows = input1->rows;
  int icols = input1->cols;
  
  Pixel ** input;
  input = malloc(irows * sizeof(*input));
  for (int i = 0 ; i < irows; i++) {
    input[i] = malloc(icols * sizeof(input[0]));
  }
  
  for (int i = 0; i < irows; i++) {
    for (int j = 0; j < icols; j++) {
      input[i][j].r = 0;
      input[i][j].g = 0;
      input[i][j].b = 0;
    }
  }
  
  
  for (int i = 0; i < irows; i++) {
    for (int j = 0; j < icols; j++) {
      input[i][j].r = input1->data[(i*(icols)) + j].r;
      input[i][j].g = input1->data[(i*(icols)) + j].g;
      input[i][j].b = input1->data[(i*(icols)) + j].b;
      // input[i][j] = input1->data[(i*(icols)) + j];
    }
  }

  

  //Beginning of the output array creation
  int orow = (irows) * 2;
  int ocol = (icols) * 2;
  
  Pixel ** output;
  output = malloc(sizeof(*output) * orow); 

  for (int i = 0; i < orow; i++) {
    output[i] = malloc(ocol * sizeof(output[0]));
  }
  //Initialize entire array before beginning zoom in process
  for (int i = 0; i < orow; i++) {
    for (int j = 0; j < ocol; j++) {
      output[i][j].r = 0;
      output[i][j].g = 0;
      output[i][j].b = 0;
    }
  }
  

  //Variables to keep track of the current row and col for input image
  int row = 0;
  int col = 0;
  //For loop only has to keep track of output image dimensions because its guaranteed to be double the input dimensions. Row and col interate once for every two iterations of output because of += 2
  
  for (int i = 0; i < orow; i += 2) {
    for (int j = 0; j < ocol; j += 2) {
      //Create a 2 x 2 box of pixels that are the same in the new output array then skip every other row and column since output array is double that of the input array
      //printf("row %d, col %d\n", row, col);
      output[i][j].r = input[row][col].r;
      output[i][j].g = input[row][col].g;
      output[i][j].b = input[row][col].b;
      
      output[i+1][j+1].r = input[row][col].r;
      output[i+1][j+1].g = input[row][col].g;
      output[i+1][j+1].b = input[row][col].b;

      output[i+1][j].r = input[row][col].r;
      output[i+1][j].g = input[row][col].g;
      output[i+1][j].b = input[row][col].b;

      output[i][j+1].r = input[row][col].r;
      output[i][j+1].g = input[row][col].g;
      output[i][j+1].b = input[row][col].b;
      
      col++;
    }
    col = 0;
    row++;
  }
  
  //Free Memory




  
  //Create return Image and convert output 2D array back to an Image
  Image * new = malloc(sizeof(Image));
  new->data = (Pixel*)malloc(sizeof(Pixel) * (orow) * (ocol));
  
  new->rows = orow;
  new->cols = ocol;
  
  for(int i = 0; i < orow; i++) {
    for(int j = 0; j < ocol; j++) {
      new->data[(i*ocol) + j] = output[i][j];
    }
  }

  //Free output after new Image is created
  for (int i = 0 ; i < orow; i++) {
    free(output[i]);
  }
  free(output);

  for (int i = 0 ; i < irows; i++) {
    free(input[i]);
  }
  free(input);
  
  
  return new;
}

Image * zoom_out(Image * input1) {
  //Declare 2D array

  int rows = input1->rows;
  int cols = input1->cols;
  int newrows = rows/2;
  int newcols = cols/2;

  
  
  
  Pixel ** input;
  input = malloc(rows * sizeof(*input));
  Pixel ** output;
  output = malloc(newrows * sizeof(*output));
  
  for (int i = 0 ; i < rows; i++) {
    input[i] = malloc(cols * sizeof(input[0]));
  }
  for (int i = 0; i < newrows; i++) {
    output[i] = malloc(newcols * sizeof(output[0]));
  }
  //Intialize 2D array
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      input[i][j].r = 0;
      input[i][j].g = 0;
      input[i][j].b = 0;
    }
  }
  for (int i = 0; i < newrows; i++) {
    for(int j = 0; j < newcols; j++) {
      output[i][j].r = 0;
      output[i][j].g = 0;
      output[i][j].b = 0;
    }
  }
  //Read input image data into 2D array
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      input[i][j].r = input1->data[(i*(cols)) + j].r;
      input[i][j].g = input1->data[(i*(cols)) + j].g;
      input[i][j].b = input1->data[(i*(cols)) + j].b;
    }
  }
  int currentrow = 0;
  int currentcol = 0;
  for(int i = 0; i < rows; i += 2) {
    for(int j = 0; j < cols; j += 2) {
      int r = (input[i][j].r + input[i+1][j].r + input[i+1][j+1].r + input[i][j+1].r)/4; 
      int g = (input[i][j].g + input[i+1][j].g + input[i+1][j+1].g + input[i][j+1].g)/4;     
      int b = (input[i][j].b + input[i+1][j].b + input[i+1][j+1].b + input[i][j+1].b)/4;
      output[currentrow][currentcols].r = r;
      output[currentrow][currentcols].g = g;
      output[currentrow][currentcols].b = b;
      currentcol++;
    }
    currentcol = 0;
    currentrow++;
  }



  
  Image * new = malloc(sizeof(Image));
  new->data = (Pixel*)malloc(sizeof(Pixel) * newrows * newcols);
  new->rows = newrows;
  new->cols = newcols;


  for (int i = 0; i < newrows; i++) {
    for (int j = 0; j < newcols; j++) {
      input1->data[(i*(newcols)) + j].r = output[i][j].r;
      input1->data[(i*(newcols)) + j].g = output[i][j].g;
      input1->data[(i*(newcols)) + j].b = output[i][j].b;
    }
  }

  for(int i = 0; i < newrows; i++) {
    free(output[i]);
  }
  free(output);

  for (int i = 0; i < rows; i++) {
    free(input[i]);
  }
  free(input);
  
  return new;
}

int doOperation(char *argv[]){
  FILE * inputF = fopen(argv[1], "rb");
  Image * inputI = read_ppm(inputF);
  FILE * outputF = fopen(argv[2], "wb");
  Image * outputI = NULL;
  int skip = 0;
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
    outputI = zoom_in(inputI);
    //skip = 1;
  }

  if(strcmp(argv[3], "zoom_out") == 0){
    outputI = zoom_out(inputI);
    skip = 1;
  }
  
  if(strcmp(argv[3], "pointilism") == 0){
    outputI = pointilism(inputI);
    //skip = 1;
  }

  if(strcmp(argv[3], "swirl") == 0){
    outputI = inputI;
    skip = 1;
  }

  if(strcmp(argv[3], "blur") == 0){
    double sigma;
    sscanf(argv[4], "%lf", &sigma);
    //outputI = blur(inputI, sigma);
    outputI = inputI;
    skip = 1;
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
  if(skip == 0){
    free(outputI->data);
    free(outputI);
  }
  fclose(inputF);
  fclose(outputF);
  free(inputI->data);
  free(inputI);
  //free(outputI->data);
  //free(outputI);
  return 0;
}
