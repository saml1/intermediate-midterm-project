/* Brayden Archer and Sam Lipschitz
 *barcher9 and slipsch3
 */
#include "ppm_io.h"
#include "imageManip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int error = error_report(argc, argv);
  if(error != 0){
    return error;
  }
  return doOperation(argv);
}
