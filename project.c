/* Brayden Archer and Sam Lipschitz
 *barcher9 and slipsch3
 */
#include "ppm_io.h"
#include "imageManip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if(argc < 3){
    printf("Error: must specify input file name and output file name.\n");
    return 1;
  }
  FILE * inputF = fopen(argv[1], "rb");
  if(inputF == NULL){
    printf("Error: could not open specified input file.\n");
    //fclose(inputF);
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
      return 6;
    }
    Image * input2_i = read_ppm(input2_f);
    if(input2_i == NULL){
      printf("Error: failed to read input file.  Must be properly-formatted PPM file.\n");
      fclose(inputF);
      free(inputI->data);
      free(inputI);
      fclose(input2_f);
      return 6;
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
