#include "pvm3.h"
#include <stdio.h>
#include <stdlib.h>
#define MSG_TAG 11
#include "supportingArrays.h"

int pvmSingleCol(int dimension, char **argv);

int main(int argc,char **argv) {

  int dimension = atoi(argv[1]);
  pvmSingleCol(dimension, argv);

return 0;
}

int pvmSingleCol(int dimension, char **argv){

    int my_tid = pvm_mytid();
    int my_parent_tid = pvm_parent();

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

      int length, tag, tid, index, info, result_value = 0;

      int *children_ids = createVector(dimension);
      int *tmp_vector = createVector(dimension);
      int *result_vector = createVector(dimension);

      printf("You are using pvmSingleCol procedure now..\n\n");
      //parent info
      printf("my tid %d\n", my_tid);
      printf("my pid %d\n", my_parent_tid);

      //prepare arguments(dimension) to be sent to all children
      char* arguments[2];
      arguments[0] = argv[1];
      arguments[1] = (char*)0;

      //spawn n tasks where n is equal to array's dimension
      int successful_children = pvm_spawn(
                                            "pvmSingleCol",
                                            (char**)arguments,
                                            PvmTaskDefault,
                                            (char*)0,
                                            dimension,
                                            children_ids
                                          );

      //spawn failed
      if(successful_children != dimension){
        printf("%s\n", "error at spawn method");
        pvm_exit();
        return -1;
      }

      /*
      sending is separate from receiving
      because we dont want all tasks to wait for one task to process
      and send the result back to us
      where task processing speed will differ from one maching to another
      */
      for(int i = 0; i < dimension; i++){

        pvm_initsend(PvmDataDefault);

        pvm_pkint( getCol( array, dimension, dimension, i ), dimension, 1);
        pvm_pkint(&vector[i], 1, 1);

        pvm_send(children_ids[i], MSG_TAG);
      }

      for(int i = 0; i < dimension; i++){

        //receive from any task which finish first
        int bufferId = pvm_recv(-1, MSG_TAG);
        if(bufferId < 0) printf("%s\n", "error at receive!");

        info = pvm_bufinfo(bufferId, &length, &tag, &tid);
        if(info < 0) printf("%s\n", "error at pvm_bufinfo...");

        info = pvm_upkint(tmp_vector, dimension, 1);
        if(info < 0) printf("%s\n", "error at un packaging...");

        //print useful info which show which task repondes first with which result
        printf("-----------\n");
        printf("vector received:\n");
        printVector(tmp_vector,dimension);
        printf("from child: %d, with id: %d\n", (tid - children_ids[0]), tid);
        // printf("\n");

        result_vector = vectorSum(result_vector, tmp_vector, dimension);
      }

      pvm_exit();

      WriteData_1D(result_vector, dimension, "pvmSingleColResult.txt");
      printf("=================================\n");
      printf("pvmSingleCol Result: \n");
      printVector(result_vector, dimension);
      printf("=================================\n");

    }
    /* children tasks */
    else{

      int *vector1 = createVector(dimension);
      int one_value;

      pvm_recv(my_parent_tid, MSG_TAG);
      pvm_upkint(vector1, dimension, 1);
      pvm_upkint(&one_value, 1, 1);

      int *vector_outcome = vectorScalar(vector1, dimension, one_value);

      pvm_initsend(PvmDataDefault);
      pvm_pkint(vector_outcome, dimension, 1);
      pvm_send(my_parent_tid, MSG_TAG);

      pvm_exit();
      return 0;
    }
return 0;
}
