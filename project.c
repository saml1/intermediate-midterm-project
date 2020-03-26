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
  //printf("%d\n", error);
  if(error == 139){
    printf("Error: unable to open specified output file for writing or writing output failed.\n");
    return 7;
  }
  if(error != 0){
    return error;
  }
  error = doOperation(argv);
  if(error == 139){
    printf("Error: unable to open specified output file for writing or writing output failed.\n");
    return 7;
  } else{
    return error;
  }
}
