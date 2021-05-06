#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
//#include <unistd.h>   // used for sleep

#include "MPI_double_layer_convolution.c"
#include "single_layer_convolution.c"



int main (int nargs, char **args)
{
  int M=0, N=0, K1=0, K2=0, my_rank;
  double t0_MPI, time_MPI, t0_seq, time_seq;
  float **input=NULL, **output=NULL, **kernel1=NULL, **kernel2=NULL;

  MPI_Init(&nargs, &args);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);



  if (my_rank==0)
  {
    // Read from command line the values of M, N, K1 and K2
    M = atof(args[1]);          // first input argument
    N = atof(args[2]);          // second input argument
    K1 = atof(args[3]);         // third input argument
    K2 = atof(args[4]);         // fourth input argument



    // Allocate 2D ’input’ array with M rows and N columns
    srand(time(0));  // use current time as seed for generating RNG numbers
    //srand(0);      // use a fixed nuber as seed for generating RNG numbers

    // Allocate the 2D array 'input' as an underlying contiguous 1D array
    input = malloc(M*sizeof(input));
    input[0] = malloc(M*N*sizeof(float));
    for (int i=1; i<M; i++)
    {
      input[i] = &(input[0][i*N]);
    }

    // Fill 'input' array with values. CNN is often used on images,
    // hence I have chosen to to fill the 'input' array with values
    // from 0-255. This is indeed not required for the MPI
    // implementation to work.
    for (int i=0; i<M; i++)
    {
      for (int j=0; j<N; j++)
      {
        input[i][j] = (int) (255.0 * rand() / RAND_MAX);
      }
    }



    // Allocate 2D ’output’ array with M-K1-K2+2 rows and N-K1-K2+2 columns
    // as an underlying contiguous 1D array
    output = malloc((M-K1-K2+2)*sizeof(output));
    output[0] = malloc((M-K1-K2+2)*(N-K1-K2+2)*sizeof(float));
    for (int i=1; i<(M-K1-K2+2); i++)
    {
      output[i] = &(output[0][i*(N-K1-K2+2)]);
    }



    // Allocate the convolutional kernels with K1/K2 rows
    // and K1/K2 columns as underlying contiguous 1D arrays

    // Allocate the 'kernel1' array
    kernel1 = malloc(K1*sizeof(kernel1));
    kernel1[0] = malloc(K1*K1*sizeof(float));
    for (int i=1; i<K1; i++)
    {
      kernel1[i] = &(kernel1[0][i*K1]);
    } // end of allocating 'kernel1'

    // Fill 'kernel1' array with values
    for (int i=0; i<K1; i++)
    {
      for (int j=0; j<K1; j++)
      {
        // Fill elements in 'kernel1' with horizontal symmetric values
        kernel1[i][j] = j - (K1/2.0 - 0.5);
      }
    } // End of filling 'kernel1' with values

    // Allocate the 'kernel2' array
    kernel2 = malloc(K2*sizeof(kernel2));
    kernel2[0] = malloc(K2*K2*sizeof(float));
    for (int i=1; i<K2; i++)
    {
      kernel2[i] = &(kernel2[0][i*K2]);
    } // End of allocating 'kernel2'

    // Fill 'kernel2' array with values
    for (int i=0; i<K2; i++)
    {
      for (int j=0; j<K2; j++)
      {
        // Fill elements in 'kernel2' with vertical symmetric values
        kernel2[i][j] = i - (K2/2.0 - 0.5);
      }
    } // End of filling 'kernel2' with values


  } // End of first 'if (my_rank==0)' block



  // Process 0 (root) broadcast values of M, N, K1,
  // K2 to all the other processes
  MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&K1, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&K2, 1, MPI_INT, 0, MPI_COMM_WORLD);



  // Allocate the convolutional kernels dynamically with K1/K2 rows
  // and K1/K2 columns on all the other processors (not root)
  if (my_rank!=0)
  {
    // Allocate the 'kernel1' array
    kernel1 = malloc(K1*sizeof(kernel1));
    kernel1[0] = malloc(K1*K1*sizeof(float));
    for (int i=1; i<K1; i++)
    {
      kernel1[i] = &(kernel1[0][i*K1]);
    } // End of allocating 'kernel1'

    // Allocate the 'kernel2' array
    kernel2 = malloc(K2*sizeof(kernel2));
    kernel2[0] = malloc(K2*K2*sizeof(float));
    for (int i=1; i<K2; i++)
    {
      kernel2[i] = &(kernel2[0][i*K2]);
    } // End of allocating 'kernel2'

  } // End of 'if (my_rank!=0)' block



  // Process 0 (root) broadcasts the content of
  // kernels to all the other processes
  MPI_Bcast(&(kernel1[0][0]), K1*K1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&(kernel2[0][0]), K2*K2, MPI_FLOAT, 0, MPI_COMM_WORLD);


  // Double layer convolution applied on the 'input' data
  // using MPI implementation
  if (my_rank==0) t0_MPI = MPI_Wtime();
  MPI_double_layer_convolution(M, N, input,
                               K1, kernel1,
                               K2, kernel2,
                               output);
  if (my_rank==0)
  {
    time_MPI = MPI_Wtime() - t0_MPI;
    printf("Elapsed timed using 'MPI_double_layer_convolution.c':   ");
    printf("%f s\n", time_MPI);
  }



  // Comparing the results from the produced MPI implementation
  // with the sequential method given in the description
  if (my_rank==0)
  {
    // Allocate two arrays 'output_test1' and 'output_test2'
    // which contains the results using the
    // 'single_layer_convolution.c' function once and
    // twice respectively
    float **output_test1, **output_test2;

    // Allocate 'output_test1' array
    output_test1 = (float**)malloc((M-K1+1)*sizeof(float*));
    for (int i=0; i<M-K1+1; i++)
    {
      output_test1[i] = (float*)malloc((N-K1+1)*sizeof(float));
    }

    // Allocate 'output_test2' array
    output_test2 = (float**)malloc((M-K1-K2+2)*sizeof(float*));
    for (int i=0; i<M-K1-K2+2; i++)
    {
      output_test2[i] = (float*)malloc((N-K1-K2+2)*sizeof(float));
    }

    // Perform convolutional computaion on 'input' array twice using
    // the sequential function
    t0_seq = MPI_Wtime();
    single_layer_convolution(M, N, input, K1, kernel1, output_test1);
    single_layer_convolution(M-K1+1, N-K1+1, output_test1, K2, kernel2, output_test2);
    time_seq = MPI_Wtime() - t0_seq;
    printf("Elapsed timed using 2x 'single_layer_convolution.c':    ");
    printf("%f s\n", time_seq);
    printf("Time consumption boost using MPI:                       ");
    printf("%f\n", time_seq/time_MPI);


    // Check that the MPI function and the sequential function
    // produce the same results by subtracting the square
    // (only positive values) of each element to each other
    float sum_elements;
    for (int i=0; i<M-K1-K2+2; i++)
    {
      for (int j=0; j<N-K1-K2+2; j++)
      {
        // Elements are squared to prevent false sum of zero
        sum_elements = sum_elements + output[i][j]*output[i][j] -
                       output_test2[i][j]*output_test2[i][j];
      }
    }
    printf("Sum of output_MPI^2-output_sequential^2:                ");
    printf("%.1f\n", sum_elements);

    // Deallocate local arrays
    free(output_test1);
    free(output_test2);

  } // End of second 'if (my_rank==0)' block



  // Deallocate global arrays
  free(input);
  free(output);
  free(kernel1);
  free(kernel2);

  // Finalize the MPI procedure
  MPI_Finalize();

  return 0;
} // End of main function
