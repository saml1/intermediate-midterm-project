Brayden Archer and Sam Lipschitz
barcher19 and slipsch3

This program can do seven operations to a given PPM file of max color value 255: exposure, blend, zoom_in, zoom_out, pointilism, swirl, and blur.

Exposure changes the exposure of the image, and requires a floating-point exposure compensation parameter between -3 and 3.

Alpha blend blends two images together and requires a floating-point blend ratio alpha between 0 and 1. This requires the filename of the second image as well as alpha.

Zoom in returns an image with double the dimensions of the input image, where each pixel of input image is duplicated into a 2x2 square. This does not require any extra parameters.

Zoom out returns an image of half the dimensions of the input image.  Every 2x2 square of pixels in the input image is replaced by a single pixel with the average color values of the four pixels in the square. This does not require any extra parameters.

Pointilism returns an image with a pointilism-like effect (pointilism is a technique in painting that uses distinct colored dots).  This does not require any extra parameters.

Swirl returns an image with a whirlpool effect.  This requires three extra parameters: the x-coordinate and y-coordinate of the pixel the swirl should be centered on and the distortion scale, a positive integer.

Blur returns a blurred image, and requires a floating-point value sigma greater than 0 that decides the how blurry the output image should be. The larger the sigma, the blurrier the image, but also the longer it will take to create the image.

The syntax is:
./project input_file.ppm output_file.ppm operation param_1 param_2 param_3
where param_1, param_2, and param_3 are not needed for every operation.

The operations