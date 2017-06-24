#include "pvm3.h"
#include <stdio.h>
#include <stdlib.h>
#define MSG_TAG 11
#include "supportingArrays.h"

int pvmMultiCol(int dimension, int children_count, char **argv);

int main(int argc,char **argv) {

  int dimension = atoi(argv[1]);
  int children_count = atoi(argv[2]);

  pvmMultiCol(dimension, children_count, argv);
  
  return 0;
}

int pvmMultiCol(int dimension, int children_count, char **argv){

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

      int added_cols = numerator_approximation(dimension, children_count);
      int new_cols = dimension + added_cols;
      int size = new_cols / children_count;

      int **new_array = expandCols_2DArray(
                                            array,
                                            dimension,
                                            dimension,
                                            added_cols
                                          );
    //  int *result_vector = createVector(size * children_count);

     printf("You are using pvmMultiCol procedure now...\n\n");
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
                                         "pvmMultiCol",
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

     //send
     for (int i = 0; i < children_count; i++) {

       pvm_initsend(PvmDataDefault);

       int **sliced = slice_2DArray(
         new_array,
         dimension,
         new_cols,
         0,
         i * size,
         dimension,
         size
       );
      //  print2DArray(sliced,dimension,size);

      int *sliced_vector = _2Dto1DArray(sliced,dimension,size);

      int *vec1 = expandVector(vector,dimension,added_cols);//shold be before the for loop
      int *vec = slice_vector(vec1,dimension+added_cols,(i*size),((i*size)+size));

      pvm_pkint(&size, 1, 1);
      pvm_pkint(sliced_vector, (size * dimension), 1);
      pvm_pkint(vec, size, 1);

      pvm_send(children_ids[i], MSG_TAG);
     }//send

     int *result_vector=createVector(dimension);

     //receive
     for (int i = 0; i < children_count; i++) {

       int *tmp_vector = createVector(dimension);

       int bufferId = pvm_recv(-1, MSG_TAG);
       if(bufferId < 0) printf("%s\n", "error at receive!");

       info = pvm_bufinfo(bufferId, &length, &tag, &tid);
       if(info < 0) printf("%s\n", "error at pvm_bufinfo...");

       info = pvm_upkint(tmp_vector, dimension, 1);
       if(info < 0) printf("%s\n", "error at un packaging...");

       index = ( tid - children_ids[0]);

       printf("--------------\n");
       printVector(tmp_vector,dimension);
       printf("is received from children N: %d\n", index);

       result_vector = vectorSum(result_vector,tmp_vector,dimension);

     }//receive

     WriteData_1D(result_vector, dimension, "pvmMultiColResult.txt");
     printf("=================================\n");
     printf("pvmMultiCol Result: \n");
     printVector(result_vector,dimension);
     printf("=================================\n");

    }
    //child code...
    else{

      int size = 0;

      pvm_recv(my_parent_tid, MSG_TAG);

      pvm_upkint(&size, 1, 1);
      int *sliced_vector = createVector(size * dimension);
      int *vec = createVector(size);

      pvm_upkint(sliced_vector, ( size * dimension ), 1);
      pvm_upkint(vec, size, 1);

      int **_2DArray = _1Dto2DArray(
                                      sliced_vector,
                                      ( size * dimension ),
                                      size
                                    );

      int *result_vector = MatVect(
                                    _2DArray,
                                    dimension,
                                    size,
                                    vec,
                                    size
                                  );

      pvm_initsend(PvmDataDefault);
      pvm_pkint(result_vector, dimension, 1);
      pvm_send(my_parent_tid, MSG_TAG);

      pvm_exit();
      return 0;
    }

}
