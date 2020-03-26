/* Brayden Archer and Sam Lipschitz
 * barcher9 and slipsch3
 * imageManip.h
 * 601.220, Spring 2019
 */

#ifndef IMAGEMANIP_H
#define IMAGEMANIP_H
#include <math.h>
#include <stdio.h>
#include "ppm_io.h"
#define PI 3.14

/* Takes Image * fp and returns a new Image * with modified exposure ev. */
Image * exposure(const Image * orig, float ev);

/* Takes two Image *'s and returns a blended image of the two with 
 * ratio alpha.
 */
Image * blend(const Image * input1, const Image * input2, float alpha);

/* Takes image and returns new Image * with a pointilism-like effect */
Image * pointilism(const Image * im);

/* Returns blurred version of given Image.
 * When sigma is larger, the output will have a stronger blurred effect.
 */
Image* blur(const Image * im, double sigma);

/* Returns the square of the given double. */
double sq(double p);

/* Returns a 2D double Gaussian matrix with variance sigma. */
double ** createGM(double sigma);

/* Returns filter response using filter gm for a Pixel in im with 
 * given row and col.
 */
Pixel* filterResponse(double ** gm, double sigma, const Image * im, int row, int col);


//Takes an input image and returns a zoomed in version of it
Image * zoom_in(const Image * input1);

//Takes an input image and returns a zoomed out version of it
Image * zoom_out(const Image * input1);

//takes an input image and returns a swirled version of it
Image * swirl(const Image * input1, const int centerX, const int centerY, const int scale);

int doOperation(char *argv[]);
#endif
