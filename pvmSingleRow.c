#include "pvm3.h"
#include <stdio.h>
#include <stdlib.h>
#define MSG_TAG 11
#include "supportingArrays.h"

int pvmSingleRow(int dimension, char **argv);

int main(int argc,char **argv) {

    int dimension = atoi(argv[1]);
    pvmSingleRow(dimension, argv);
    
return 0;
}

int pvmSingleRow(int dimension, char **argv){


      int my_tid = pvm_mytid();
      int my_parent_tid = pvm_parent();

      /* tasks' parent*/
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
        int *result_vector = createVector(dimension);

        printf("You are using pvmSingleRow procedure now..\n\n");
        //parent info
        printf("my tid %d\n", my_tid);
        printf("my pid %d\n\n", my_parent_tid);

        //prepare arguments(dimension) to be sent to all children
        char* arguments[2];
        arguments[0] = argv[1];
        arguments[1] = (char*)0;

        //spawn n tasks where n is equal to array's dimension
        int successful_children = pvm_spawn(
                                            "pvmSingleRow",
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

          pvm_pkint( getRow( array, dimension, dimension, i ), dimension, 1);
          pvm_pkint(vector, dimension, 1);

          pvm_send(children_ids[i], MSG_TAG);
        }


        for(int i = 0; i < dimension; i++){

          //receive from any task which finish first
          int bufferId = pvm_recv(-1, MSG_TAG);
          if(bufferId < 0) printf("%s\n", "error at receive!");

          info = pvm_bufinfo(bufferId, &length, &tag, &tid);
          if(info < 0) printf("%s\n", "error at pvm_bufinfo...");

          info = pvm_upkint(&result_value, 1, 1);
          if(info < 0) printf("%s\n", "error at un packaging...");

          //print useful info which show which task repondes first with which result
          printf("received: [%5d ], from child: %d, with id: %d\n", result_value, (tid - children_ids[0]), tid);
          printf("---------------\n");
          //we need "index" in case we received responses from children "unordered"
          index = tid - children_ids[0];
          result_vector[index] = result_value;
        }

        pvm_exit();

        WriteData_1D(result_vector, dimension, "pvmSingleRowResult.txt");
        printf("=================================\n");
        printf("pvmSingleRow Result: \n");
        printVector(result_vector, dimension);
        printf("=================================\n");

      }
      /* children tasks */
      else{

        int *vector1 = createVector(dimension);
        int *vector2 = createVector(dimension);

        pvm_recv(my_parent_tid, MSG_TAG);
        pvm_upkint(vector1, dimension, 1);
        pvm_upkint(vector2, dimension, 1);

        int result_value = vectorXvector(vector1, vector2, dimension);

        pvm_initsend(PvmDataDefault);
        pvm_pkint(&result_value,1,1);
        pvm_send(my_parent_tid, MSG_TAG);

        pvm_exit();
        return 0;
      }

  return 0;
}
