#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "supportingArrays.h"
#include <time.h>

int main(int argc, char const *argv[]) {

  FILE* fileDescriptor = fopen("time_results.txt", "w");

  int dimension;
  printf("please enter dimension wanted(integer)\n");
  scanf("%d", &dimension);

  int choice;
  while(choice != 1 && choice != 2){
    printf("do you want\n1-auto generated array and vector \n");
    printf("2-load array and vector elements from .txt file?\n");
    printf("choose 1 or 2...\n");
    scanf("%d", &choice);
  }
  if(choice == 1){

    int **array = RandomGenerator_2D(dimension, dimension);
    WriteData(array, dimension, dimension, "readArray.txt");


    int **vector = RandomGenerator_2D(dimension, 1);
    WriteData(vector, dimension, 1, "readVector.txt");

  }
  if(choice == 2){

    int rows1, rows2, cols1, cols2;

    int **array = ReadData(&rows1, &cols1, "readArray.txt");
    int **vector = ReadData(&rows2, &cols2, "readVector.txt");

    if(!((rows1 == rows2*cols2)&&(rows1 == dimension))){
      printf("you didnt write proper sized array or vector in your text files\n");

      return -1;
    }
  }

  int rows1, cols1;
  int **array = ReadData(&rows1, &cols1, "readArray.txt");
  printf("Array:\n");
  print2DArray(array, rows1, cols1);

  int rows2, cols2;
  int **tmp2 = ReadData(&rows2, &cols2, "readVector.txt");
  int *vector = _2Dto1DArray(tmp2,rows2,cols2);
  printf("Vector: \n");
  printVector(vector, rows2*cols2);
  
  choice = 0;
  int choice2 = 0;

  while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5){
    printf("choose 2 between these 5 procedure to multiply:\n");
    printf("1-pvm single row\n");
    printf("2-pvm single col\n");
    printf("3-pvm multi row\n");
    printf("4-pvm multi col\n");
    printf("5-no pvm(use matVect(serial multiplication))\n");
    scanf("%d", &choice);
  }

  choice2 = choice;
  choice = 0;

  while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5){
    printf("choose 1 between these 5 procedure to multiply:\n");
    printf("1-pvm single row\n");
    printf("2-pvm single col\n");
    printf("3-pvm multi row\n");
    printf("4-pvm multi col\n");
    printf("5-no pvm(use matVect(serial multiplication))\n");
    scanf("%d", &choice);
    if(choice == choice2)
      choice = 0;
  }


////////////////
if(choice == 1 || choice2 == 1){

  char str[100];
  sprintf(str, "cc pvmSingleRow.c -o pvmSingleRow *.a;./pvmSingleRow %d", dimension);

  clock_t begin_pvmSingleRow = clock();

  int pvmSingleRowStatus = system(str);

  clock_t end_pvmSingleRow = clock();

  double time_spent_pvmSingleRow = (double)(end_pvmSingleRow - begin_pvmSingleRow) / CLOCKS_PER_SEC;

  fprintf(fileDescriptor,"time_spent_pvmSingleRow: %7f\n\n", time_spent_pvmSingleRow);
  printf("time_spent_pvmSingleRow: %7f\n\n", time_spent_pvmSingleRow);

}
////////////////
if(choice == 2 || choice2 == 2){

  char str[100];
  sprintf(str, "cc pvmSingleCol.c -o pvmSingleCol *.a;./pvmSingleCol %d", dimension);

  clock_t begin_pvmSingleCol = clock();

  int pvmSingleColStatus = system(str);

  clock_t end_pvmSingleCol = clock();

  double time_spent_pvmSingleCol = (double)(end_pvmSingleCol - begin_pvmSingleCol) / CLOCKS_PER_SEC;

  fprintf(fileDescriptor,"time_spent_pvmSingleCol: %7f\n\n", time_spent_pvmSingleCol);
  printf("time_spent_pvmSingleCol: %7f\n\n", time_spent_pvmSingleCol);

}
////////////////

if(choice == 3 || choice2 == 3){

  int children_count = -1;
  while( children_count <= 0 ){
    printf("insert children count to to work together on pvmMultiRow\n");
    scanf("%d", &children_count);
  }

  char str[100];
  sprintf(str, "cc pvmMultiRow.c -o pvmMultiRow *.a;./pvmMultiRow %d %d", dimension, children_count);

  clock_t begin_pvmMultiRow = clock();

  int pvmMultiRowStatus = system(str);

  clock_t end_pvmMultiRow = clock();

  double time_spent_pvmMultiRow = (double)(end_pvmMultiRow - begin_pvmMultiRow) / CLOCKS_PER_SEC;

  fprintf(fileDescriptor,"time_spent_pvmMultiRow:  %7f\n\n", time_spent_pvmMultiRow);
  printf("time_spent_pvmMultiRow:  %7f\n\n", time_spent_pvmMultiRow);

}
////////////////

if(choice == 4 || choice2 == 4){

  int children_count = -1;
  while( children_count <= 0 ){
    printf("insert children count to to work together on pvmMultiCol\n");
    scanf("%d", &children_count);
  }

  char str[100];
  sprintf(str, "cc pvmMultiCol.c -o pvmMultiCol *.a;./pvmMultiCol %d %d", dimension, children_count);

  clock_t begin_pvmMultiCol = clock();

  int pvmMultiColStatus = system(str);

  clock_t end_pvmMultiCol = clock();

  double time_spent_pvmMultiCol = (double)(end_pvmMultiCol - begin_pvmMultiCol) / CLOCKS_PER_SEC;

  fprintf(fileDescriptor,"time_spent_pvmMultiCol:  %7f\n\n", time_spent_pvmMultiCol);
  printf("time_spent_pvmMultiCol:  %7f\n\n", time_spent_pvmMultiCol);

}

////////////////

if(choice == 5 || choice2 == 5){

  clock_t begin_matVect = clock();

  int matVectStatus =
  system("cc matVect.c -o matVect;./matVect");

  clock_t end_matVect = clock();

  double time_spent_matVect = (double)(end_matVect - begin_matVect) / CLOCKS_PER_SEC;

  fprintf(fileDescriptor,"time_spent_matVect:      %7f\n\n", time_spent_matVect);
  printf("time_spent_matVect:      %7f\n\n", time_spent_matVect);

}
////////////////

  fclose(fileDescriptor);
  printf("\ntime results are saved at time_results.txt file\n");
  return 0;
}
