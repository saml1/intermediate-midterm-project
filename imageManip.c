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
  Image * new = malloc(sizeof(Image));
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
  }
  //randomly picking 3% of pixels and applying pointilism effect
  int curx = 0;
  int cury = 0;
  int curr = 0;
  for(int i = 0; i < im->rows; i++){
    for(int j = 0; j < im->cols; j++){
      pix2d[i][j].r= im->data[(i * im->cols) + j].r;
      pix2d[i][j].g= im->data[(i * im->cols) + j].g;
      pix2d[i][j].b= im->data[(i * im->cols) + j].b;
    }
  }
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
Image* blur(const Image * im, double sigma) {
  //Create a new output Image
  Image * new = malloc(sizeof(Image));
  new->rows = im->rows;
  new->cols = im->cols;
  new->data = malloc(sizeof(Pixel) * im->rows * im->cols);
  //Create a 2D array of Pixels to edit for output
  Pixel ** input;
  input = malloc(sizeof(*input) * im->rows);
  for (int i = 0; i < im->rows; i++) {
    input[i] = malloc(im->cols * sizeof(input[0]));
  }
  //Write the original image to the output array before editing
  for(int i = 0; i < im->rows; i++) {
    for(int j = 0; j < im->cols; j++) {
      input[i][j] = im->data[(i*im->cols) + j];
    }
  }
  //Create a 2D matrix to edit
  Pixel ** output;
  output = malloc(sizeof(*output) * im->rows);
  for(int i = 0; i < im->rows; i++) {
    output[i] = malloc(sizeof(output[0]) * im->cols);
  }
  for(int i = 0; i < im->rows; i++) {
    for(int j = 0; j < im->cols; j++) {
      output[i][j] = im->data[(i*im->cols) + j];
    }
  }
  
  //Create Gaussian Matrix
  int S = sigma * 10; //Create a variable for size of matrix
  if(S % 2 == 0) {
    S += 1;
  }
  int R = (S-1) / 2; //Define a center point
  double O = sigma; //Define sigma as O for simplification of formula
  double pi = 3.141592653589793; //Pi with 15 digits of accuracy

  double ** gm = malloc(S * sizeof(double));
  for(int i = 0; i < S; i++) {
    gm[i] = malloc(S * sizeof(double));
  }
  double sumGM = 0.0;
  for(int i = 0; i < S; i++) {
    for(int j = 0; j< S; j++) {
      double dx = abs(S/2 - j);
      double dy = abs(S/2 - i);
      gm[i][j] = (1.0 / (2.0 * pi * sq(O))) * exp(-(sq(dx) + sq(dy)) / (2* sq(O)));
    }
  }
  double sumR = 0;
  double sumG = 0;
  double sumB = 0;
  //Run gaussian matrix over output array
  for(int u = 0; u < im->rows; u++) {
    for(int v = 0; v < im->cols; v++) {
      for(int i = 0; i < S; i++) {
	for(int j = 0; j < S; j++) {
	  if(((u + i - R) >= 0) && ((u + i - R) < im->rows) && ((v + j - R) >= 0) && ((v + j - R) < im->cols)) {
	    sumR += gm[i][j] * input[u + i - R][v + j - R].r;
	    sumG += gm[i][j] * input[u + i - R][v + j - R].g;
	    sumB += gm[i][j] * input[u + i - R][v + j - R].b;
	    sumGM += gm[i][j];
	  }
	}
      }
      output[u][v].r = (unsigned char)(sumR / sumGM);
      output[u][v].g = (unsigned char)(sumG / sumGM);
      output[u][v].b = (unsigned char)(sumB / sumGM);
      sumGM = 0;
      sumR = 0;
      sumG = 0;
      sumB = 0;
    }
  }

  //Write the edited output array into the new image to return
  for(int i = 0; i < im->rows; i++) {
    for(int j = 0; j < im->cols; j++) {
      new->data[(i*im->cols) + j] = output[i][j];
    }
  }
  //Free Memory
  for(int i = 0;i < im->rows; i++) {
    free(output[i]);
    free(input[i]);
  }
  for(int i = 0; i < S; i++) {
    free(gm[i]);
  }
  free(gm);
  free(input);
  free(output);
  return new;
  
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

  //Free memory
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

Image * zoom_out(const Image * input1) {
  //Declare variables that are easy to remember
  int rows = input1->rows;
  int cols = input1->cols;
  int newrows = rows/2;
  int newcols = cols/2;

  //Declare input 2D array and output
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
      input[i][j].g = 255;
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
      //Average the pixels in a box and set the average equal to the current position in the output array
      

      int r = (input[i][j].r + input[i+1][j].r + input[i+1][j+1].r + input[i][j+1].r)/4; 
      int g = (input[i][j].g + input[i+1][j].g + input[i+1][j+1].g + input[i][j+1].g)/4;     
      int b = (input[i][j].b + input[i+1][j].b + input[i+1][j+1].b + input[i][j+1].b)/4;
      
      output[currentrow][currentcol].r = r;
      output[currentrow][currentcol].g = g;
      output[currentrow][currentcol].b = b;
      
      currentcol++;
    }
    currentcol = 0;
    currentrow++;
  }
  
  //Allocate memory for new iamge to return
  Image * new = malloc(sizeof(Image));
  new->data = (Pixel*)malloc(sizeof(Pixel) * (newrows) * (newcols));
  new->rows = newrows;
  new->cols = newcols;

  //Convert output array to Image
  for (int i = 0; i < newrows; i++) {
    for (int j = 0; j < newcols; j++) {
      new->data[(i*(newcols)) + j].r = output[i][j].r;
      new->data[(i*(newcols)) + j].g = output[i][j].g;
      new->data[(i*(newcols)) + j].b = output[i][j].b;
    }
  }
  
  //Free memory
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

/* Returns the square of the given double. */
double sq(double p){
  return p * p;
}

Image * swirl(const Image * input1, const int centerX, const int centerY, const int scale) {
  //Declare some variables with easy names to remember
  int rows = input1->rows;
  int cols = input1->cols;
  int newrows = rows;
  int newcols = cols;

  
  
  //Declare original 2D arrays
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
      input[i][j].g = 255;
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

  //Loop through all coordinates of input image
  for (int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      //Main Algorithm X and Y to simplify equations
      int X = j;
      int Y = i;
      
      //Alpha declared to simplify pixel calculations
      double alpha = (sqrt(sq(X - centerX) + sq(Y - centerY) ) ) / (scale);
      
      //New X and Y declared and calculated
      int newX = (X - centerX) * cos(alpha) - (Y - centerY) * sin(alpha) + centerX;
      int newY = (X - centerX) * sin(alpha) + (Y - centerY) * cos(alpha) + centerY;
      
      //Verifies that newX and newY are valid coordinates
      if(newX >= 0 && newX < cols && newY >= 0 && newY < rows) {
	output[i][j] = input[newY][newX];
      }
    }
  }

  //Declare and allocate memory for new image
  Image * new = malloc(sizeof(Image));
  new->data = (Pixel*)malloc(sizeof(Pixel) * newrows * newcols);
  new->rows = newrows;
  new->cols = newcols;
  
  //Convert output array to a 1D array for image
  for(int i = 0; i < newrows; i++) {
    for(int j = 0; j < newcols; j++) {
      new->data[(i*newcols) + j] = output[i][j];
    }
  }
  //Free allocated memory
  for (int i = 0; i < newrows; i++) {
    free(output[i]);
  }
  free(output);

  for(int i = 0; i < rows; i++) {
    free(input[i]);
  }
  free(input);
  
  //Return the new image
  return new;
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
    outputI = zoom_in(inputI);
  }

  if(strcmp(argv[3], "zoom_out") == 0){
    outputI = zoom_out(inputI);
  }
  
  if(strcmp(argv[3], "pointilism") == 0){
    outputI = pointilism(inputI);
  }

  if(strcmp(argv[3], "swirl") == 0){
    int centerX = 0;
    int centerY = 0;
    int scale = 0;
    sscanf(argv[4], "%d", &centerX);
    sscanf(argv[5], "%d", &centerY);
    sscanf(argv[6], "%d", &scale);
    outputI = swirl(inputI, centerX, centerY, scale);
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
