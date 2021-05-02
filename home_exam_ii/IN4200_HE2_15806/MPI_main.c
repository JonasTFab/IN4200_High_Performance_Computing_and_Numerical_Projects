#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>   // used for sleep

#include "MPI_double_layer_convolution.c"
#include "single_layer_convolution.c"



int main (int nargs, char **args)
{
  int M=0, N=0, K1=0, K2=0, my_rank;
  float **input=NULL, **output=NULL, **kernel1=NULL, **kernel2=NULL;

  MPI_Init(&nargs, &args);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);



  if (my_rank==0)   // DONE
  {
    //DONE read from command line the values of M, N, K1 and K2
    //DONE allocate 2D array ’input’ with M rows and N columns
    //DONE allocate 2D array ’output’ with M-K1-K2+2 rows and N-K1-K2+2 columns
    //DONE allocate the convolutional kernels with K1(K2) rows and K1(K2) columns
    //DONE fill 2D array ’input’ with some values
    //DONE fill kernels with some values
    // ....

    M = atof(args[1]);          // first input argument
    N = atof(args[2]);          // second input argument
    K1 = atof(args[3]);         // third input argument
    K2 = atof(args[4]);         // fourth input argument


    //srand(time(0));  // use current time as seed for generating RNG numbers
    srand(0);  // use a fixed nuber as seed for generating RNG numbers
    // allocate the 2D array 'input' with underlying contigupus memory
    input = malloc(M * sizeof(input));
    input[0] = malloc(M * N * sizeof(float));
    for (int i=1; i<M; i++)
    {
      input[i] = &(input[0][i*N]);
    }

    // fill 'input' array with values
    for (int i=0; i<M; i++)
    {
      for (int j=0; j<N; j++)
      {
        input[i][j] = (int) (255.0 * rand() / RAND_MAX);
      }
    }



    // allocate the 2D array 'output'
    output = (float**)malloc((M-K1-K2+2)*sizeof(float*));
    for (int i=0; i<(M-K1-K2+2); i++)
    {
      output[i] = (float*)malloc((N-K1-K2+2)*sizeof(float));
    } // end of allocating 'output'



    // allocate the 'kernel1' array and fill it with values
    kernel1 = (float**)malloc(K1*sizeof(float*));
    for (int i=0; i<K1; i++)
    {
      kernel1[i] = (float*)malloc(K1*sizeof(float));
      for (int j=0; j<K1; j++)
      {
        // fill elements in kernel1 with horizontal symmetric values
        kernel1[i][j] = j - (K1/2.0 - 0.5);
      }
    } // end of allocating 'kernel1'



    // allocate the 'kernel2' array and fill it with values
    kernel2 = (float**)malloc(K2*sizeof(float*));
    for (int i=0; i<K2; i++)
    {
      kernel2[i] = (float*)malloc(K2*sizeof(float));
      for (int j=0; j<K2; j++)
      {
        // fill elements in kernel2 with vertical symmetric values
        kernel2[i][j] = i - (K2/2.0 - 0.5);
      }
    } // end of allocating 'kernel2'

  } // end of first 'if (my_rank==0)' block



  //DONE process 0 (root) broadcasts values of M, N, K1, K2 to all the other processes
  // ...

  //MPI_Barrier(MPI_COMM_WORLD); // making sure broadcast does not happen to early
  MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&K1, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&K2, 1, MPI_INT, 0, MPI_COMM_WORLD);



  if (my_rank!=0)   // DONE
  {
    //DONE allocated the convolutional kernels with K1(K2) rows and K1(K2) columns
    // ...

    // allocate the 'kernel1' array in other processors
    kernel1 = (float**)malloc(K1*sizeof(float*));
    for (int i=0; i<K1; i++)
    {
      kernel1[i] = (float*)malloc(K1*sizeof(float));
    } // end of allocating 'kernel1'


    // allocate the 'kernel2' array in other processors
    kernel2 = (float**)malloc(K2*sizeof(float*));
    for (int i=0; i<K2; i++)
    {
      kernel2[i] = (float*)malloc(K2*sizeof(float));
    } // end of allocating 'kernel2'

  } // end of 'if (my_rank!=0)' block



  //DONE process 0 (root) broadcasts the content of kernels to all the other processes
  // ...

  for (int i=0; i<K1; i++)
  {
    MPI_Bcast(&(kernel1[i][0]), K1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }

  for (int i=0; i<K2; i++)
  {
    MPI_Bcast(&(kernel2[i][0]), K2, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }



  // parallel computation of a double-layer convolution

  MPI_double_layer_convolution(M, N, input,
                               K1, kernel1,
                               K2, kernel2,
                               output);

  MPI_Barrier(MPI_COMM_WORLD); // wait with testing untill all proceccors are done




  sleep(1);
  if (my_rank==0)
  {
    // For example, compare the content of array ’output’ with that is
    // produced by the sequential function 'single_layer_convolution'
    // ...


    //    ####### Start of sequential #######

    // run code with an sequential approach
    float **output_test1, **output_test2;

    // allocate 'output_test1' array
    output_test1 = (float**)malloc((M-K1+1)*sizeof(float*));
    for (int i=0; i<M-K1+1; i++)
    {
      output_test1[i] = (float*)malloc((N-K1+1)*sizeof(float));
    }

    // allocate 'output_test2' array
    output_test2 = (float**)malloc((M-K1-K2+2)*sizeof(float*));
    for (int i=0; i<M-K1-K2+2; i++)
    {
      output_test2[i] = (float*)malloc((N-K1-K2+2)*sizeof(float));
    }

    single_layer_convolution(M, N, input, K1, kernel1, output_test1);
    printf("\n\nPrint 'output_test1' array as a results of using a ");
    printf("single layer convolution (sequential):\n\n");
    for (int i=0; i<M-K1+1; i++)
    {
      for (int j=0; j<N-K1+1; j++)
      {
        printf("%f  ", output_test1[i][j]);
      }
      printf("\n");
    }
    /*
    single_layer_convolution(M-K1+1, N-K1+1, output_test1, K2, kernel2, output_test2);
    printf("Print 'output_test2' array as a results of using a ");
    printf("double layer convolution (sequential):\n\n");
    for (int i=0; i<M-K1-K2+2; i++)
    {
      for (int j=0; j<N-K1-K2+2; j++)
      {
        printf("%f  ", output_test2[i][j]);
      }
      printf("\n");
    }
    */

    //    ####### End of sequential #######



    //    ####### Start of comparing #######
    /*
    float sum_elements;
    for (int i=0; i<M-K1-K2+2; i++)
    {
      for (int j=0; j<N-K1-K2+2; j++)
      {
        // elements are squared to prevent false sum of zero
        sum_elements = sum_elements + output[i][j]*output[i][j] -
                       output_test2[i][j]*output_test2[i][j];
      }
    }
    printf("sum of output_MPI-output_sequential: %f\n", sum_elements);
    free(output_test1);
    free(output_test2);
    */
    //    ####### End of comparing #######




  } // end of second 'if (my_rank==0)' block



  // deallocate arrays
  free(input);
  free(output);
  free(kernel1);
  free(kernel2);

  MPI_Finalize();


  return 0;
} // end of main function
