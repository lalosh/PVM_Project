#include "pvm3.h"
#include <stdio.h>
#include <stdlib.h>
#define MSG_TAG 11
#include "supportingArrays.h"

int pvmMultiRow(int dimension, int children_count, char **argv);

int main(int argc,char **argv) {

  int dimension = atoi(argv[1]);
  int children_count = atoi(argv[2]);

  pvmMultiRow(dimension, children_count, argv);

  return 0;
}

int pvmMultiRow(int dimension, int children_count, char **argv){

  //register with pvmd
  int my_tid = pvm_mytid();
  int my_parent_tid = pvm_parent();

  //I am the parent => parent code
  if( my_parent_tid == PvmNoParent ){

    int rows1, cols1;
    int **array = ReadData(&rows1, &cols1, "readArray.txt");

    if(!( (rows1 == dimension) && (cols1 == dimension) )){
      fprintf(stderr, "%s\n", "*you didnt match the writen array in terms of dimension");
      return -1;
    }

    int rows2, cols2;
    int **tmp2 = ReadData(&rows2, &cols2, "readVector.txt");
    int *vector = _2Dto1DArray(tmp2,rows2,cols2);

    if(!((rows2 == dimension)&&(cols2 == 1))){
      fprintf(stderr, "%s\n", "**you didnt match the writen array in terms of dimension");
      return -1;
    }

    int length, tag, tid, info, index;
    int *children_ids = createVector(children_count);

    int added_rows = numerator_approximation(dimension, children_count);
    int new_rows = dimension + added_rows;
    int size = new_rows / children_count;

    int **new_array = expandRows_2DArray(
                                          array,
                                          dimension,
                                          dimension,
                                          added_rows
                                        );

    int *result_vector = createVector(size * children_count);

    printf("You are using pvmMultiRow procedure now...\n\n");
    //parent info
    printf("my tid %d\n", my_tid);
    printf("my pid %d\n", my_parent_tid);

    //prepare arguments(dimension and children_count) to be sent to all children
    char* arguments[3];
    for (int i = 0; i < 2; i++) {
      arguments[i] = argv[i+1];
    }
    arguments[2] = (char*)0;

    int successful_children = pvm_spawn(
                                        "pvmMultiRow",
                                        (char**)arguments,
                                        PvmTaskDefault,
                                        (char*)0,
                                        children_count,
                                        children_ids
                                      );

    if(successful_children != children_count){
      printf("%s\n", "error at spawn method");
      pvm_exit();
      return -1;
    }

    //send rows (equal to 'size') along with 'vector' to my children
    for (int i = 0; i < children_count; i++) {

      pvm_initsend(PvmDataDefault);

      int **sliced = slice_2DArray(
        new_array,
        new_rows,
        dimension,
        i * size,
        0,
        size,
        dimension
      );
      int *sliced_vector = _2Dto1DArray(sliced, size, dimension);

      pvm_pkint(&size, 1, 1);
      pvm_pkint(sliced_vector, (size * dimension), 1);
      pvm_pkint(vector, dimension, 1);

      pvm_send(children_ids[i], MSG_TAG);

    }//sending finish

    //receive multi vectors with dimension equal to "size" from my children
    for (int i = 0; i < children_count; i++) {

      int *tmp_vector = createVector(size);

      int bufferId = pvm_recv(-1, MSG_TAG);
      if(bufferId < 0) printf("%s\n", "error at receive!");

      info = pvm_bufinfo(bufferId, &length, &tag, &tid);
      if(info < 0) printf("%s\n", "error at pvm_bufinfo...");

      info = pvm_upkint(tmp_vector, size, 1);
      if(info < 0) printf("%s\n", "error at un packaging...");

      index = ( tid - children_ids[0]);

      printf("--------------\n");
      printVector(tmp_vector, size);
      printf("is received from children N: %d\n", index);

      place_vector(
                   result_vector, size * children_count, /*result_vector along with its dimension*/
                   tmp_vector, size, index /*place tmp_vector inside result_vector where position = 'index'*/
                  );

    }//receiving finish

    //final stage
    int *final_result_vector = drop_from_vector(
                                                result_vector, /*input vector*/
                                                (size * children_count),/*input vector dimension*/
                                                added_rows /*wanted rows to be droped*/
                                               );

    pvm_exit();

    WriteData_1D(final_result_vector, dimension, "pvmMultiRowResult.txt");
    printf("=================================\n");
    printf("pvmMultiRow Result: \n");
    printVector(final_result_vector,dimension);
    printf("=================================\n");

  }

  //children code...
  else{

    int size = 0;
    int *vector = createVector(dimension);

    //receiving size and the sliced_vector and the vector to multiply with
    pvm_recv(my_parent_tid, MSG_TAG);

    pvm_upkint(&size, 1, 1);
    int *sliced_vector = createVector(size * dimension);

    pvm_upkint(sliced_vector, ( size * dimension ), 1);
    pvm_upkint(vector, dimension, 1);

    int **_2DArray = _1Dto2DArray( sliced_vector, ( size * dimension ), dimension );

    int *result_vector = MatVect(
                                  _2DArray,
                                  size,         /*rows of _2DArray*/
                                  dimension, /*columns of _2DArray*/
                                  vector,
                                  dimension  /*vector dimension*/
                                );
    //sending result to my parent
    pvm_initsend(PvmDataDefault);
    pvm_pkint(result_vector, size, 1);
    pvm_send(my_parent_tid, MSG_TAG);

    pvm_exit();
    return 0;
  }

return 0;
}
