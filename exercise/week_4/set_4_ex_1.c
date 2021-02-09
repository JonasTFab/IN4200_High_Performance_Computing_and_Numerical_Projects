#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>



void measure_memory_bandwidth()
{
  double s = 1.2;
  int N = 100000;
  double *A = malloc(N * sizeof A);
  double *B = malloc(N * sizeof B);

  // assigning values in vector B
  // include -lm when compiling so that 'sin()' function works
  for (int i=0; i<N; i++)
  {
    B[i] = sin((double)0.05*i);
  }

  time_t t0 = clock();
  // the SCALE stream micro-benchmark (DP words = 2(3))
  for (int i=0; i<N; i++)
  {
    A[i] = s*B[i];
  }
  time_t t_end = clock();
  double tot_time = (double)(t_end - t0) / CLOCKS_PER_SEC;

  printf("Bytes per second: %.3e\n", (2 * N * sizeof *A) / tot_time);

  free(A);
  free(B);
}



void stride_access_of_arrays()
{
  double s = 1.2;
  int N = 100000;
  int stride = 1;        // try 1-8
  double *A = malloc(N * sizeof A);
  double *B = malloc(N * sizeof B);

  // assigning values in vector B
  // include -lm when compiling so that 'sin()' function works
  for (int i=0; i<N; i++)
  {
    B[i] = sin((double)0.05*i);
  }

  time_t t0 = clock();
  // the SCALE stream micro-benchmark (DP words = 2(3))
  for (int i=0; i<N; i+=stride)
  {
    A[i] = s*B[i];
  }
  time_t t_end = clock();

  double tot_time = (double)(t_end - t0) / CLOCKS_PER_SEC;

  printf("Bytes per second (with stride): %.3ebps\n",
         (2 * (int)(N/((double)stride+0.5)) * sizeof *A) / tot_time);

  free(A);
  free(B);

}


double **set_up_2D_array(double **arr, int dim1, int dim2)
{

  arr = (double**)malloc(dim1 * sizeof(double*));

  for (int i=0; i<dim1; i++)
  {
    arr[i] = (double*)malloc(dim2 * sizeof(double));
  }

  return arr;
}



double **produce_values_in_2D_array(double **arr, int dim1, int dim2)
{

  //printf("\n");
  for (int i=0; i<dim1; i++)
  {
    for (int j=0; j<dim2; j++)
    {
      arr[i][j] = (i+1) * exp(-i/dim1) * sin(i+j);
      //printf("arr[%d][%d] = %f\n", i, j, arr[i][j]);
    }
  }

  return arr;
}




void matrix_matrix_multiplication()
{
  int m = 3;
  int n = 2;
  int p = 4;
  printf("[A] = (%d,%d)\n", n, m);
  printf("[B] = (%d,%d)\n", m, p);
  printf("[C] = (%d,%d)\n\n", n, p);

  double **A, **B, **C;

  // allocates the A, B and C matrices
  A = set_up_2D_array(A, n, m);
  B = set_up_2D_array(B, m, p);
  C = set_up_2D_array(C, n, p);


  // produce values in A and B
  A = produce_values_in_2D_array(A, n, m);
  B = produce_values_in_2D_array(B, m, p);


  // perform the matrix multiplication
  //printf("Matrix C:\n");
  for (int i=0; i<n; i++)
  {
    for (int j=0; j<p; j++)
    {
      for (int k=0; k<m; k++)
      {
        C[i][j] += A[i][k]*B[k][j];
      }
    //printf("%.1f  ", C[i][j]);
    }
  //printf("\n");
  }


  // frees the allocated matrices
  free(A);
  free(B);
  free(C);




  // don't get the parts below (task 3b) in exercise set)
  int M = 64 / (sizeof(double));       // cache size
  int L_C = 64 / (sizeof(double));     // length of cache line

  // for the case where matrices fit in cache
  m = 1;
  n = 2;
  p = 2;
  int size_of_all_matrices = n*m + m*p + n*p;

  int loads = (n*m + m*p) / L_C;
  int stores = (n*p) / L_C;

  printf("Small matrices:\n");
  printf("M=%d   length of cache=%d   size of matrices=%d\n", M, L_C, size_of_all_matrices);
  printf("loads=%d    stores=%d\n", loads, stores);



  // for the case where matrices do not fit in cache
  m = 4;
  n = 2;
  p = 3;
  size_of_all_matrices = n*m + m*p + n*p;

  loads = n*m / L_C + n*m*p;
  stores = (n*p) / L_C;

  printf("\nLarge matrices:\n");
  printf("M=%d   length of cache=%d   size of matrices=%d\n", M, L_C, size_of_all_matrices);
  printf("loads=%d    stores=%d\n", loads, stores);





  // loop unrolling
  m = 100;
  n = 100;
  p = 100;

  A = set_up_2D_array(A, n, m);
  B = set_up_2D_array(B, m, p);
  C = set_up_2D_array(C, n, p);
  A = produce_values_in_2D_array(A, n, m);
  B = produce_values_in_2D_array(B, m, p);

  // initial loop approach
  time_t t_start = clock();
  for (int i=0; i<n; i++)
  {
    for (int j=0; j<p; j++)
    {
      for (int k=0; k<m; k++)
      {
        C[i][j] += A[i][k]*B[k][j];
      }
    }
  }
  time_t t_stop = clock();

  double time_init = (double) (t_stop-t_start) / CLOCKS_PER_SEC;


  int enroll = 4;
  int p_rest = p%enroll;

  // loop unrolling approach
  t_start = clock();
  for (int i=0; i<n; i++)
  {
    for (int j=0; j<p-p_rest; j+=enroll)
    {
      for (int k=0; k<m; k++)
      {
        C[i][j]   += A[i][k]*B[k][j];
        C[i][j+1] += A[i][k]*B[k][j+1];
        C[i][j+2] += A[i][k]*B[k][j+2];
        C[i][j+3] += A[i][k]*B[k][j+3];
        //C[i][j+4] += A[i][k]*B[k][j+4];
        //C[i][j+5] += A[i][k]*B[k][j+5];
        //C[i][j+6] += A[i][k]*B[k][j+6];
        //C[i][j+7] += A[i][k]*B[k][j+7];
        //C[i][j+8] += A[i][k]*B[k][j+8];
        //C[i][j+9] += A[i][k]*B[k][j+9];
        //C[i][j+10] += A[i][k]*B[k][j+10];
        //C[i][j+11] += A[i][k]*B[k][j+11];
        //C[i][j+12] += A[i][k]*B[k][j+12];
        //C[i][j+13] += A[i][k]*B[k][j+13];
        //C[i][j+14] += A[i][k]*B[k][j+14];
        //C[i][j+15] += A[i][k]*B[k][j+15];
      }
    }

    // rest of p values
    for (int j=p-p_rest; j<p; j++)
    {
      for (int k=0; k<m; k++)
      {
        C[i][j]   += A[i][k]*B[k][j];
      }
    }
  }
  t_stop = clock();

  double time_unroll = (double) (t_stop-t_start) / CLOCKS_PER_SEC;


  printf("Time used with initial approach:   %.2fms\n", 1000*time_init);
  printf("Time used with unrolling:          %.2fms\n", 1000*time_unroll);
  printf("Time usage boost:                  %.4f\n", time_init/time_unroll);


  free(A);
  free(B);
  free(C);

}





int main()
{

  // measue memory bandwidth (Not sure if correct approach or not)
  //measure_memory_bandwidth();

  // utilising stride
  //stride_access_of_arrays();

  // matrix multiplication (linear algebra)
  //matrix_matrix_multiplication();





  return 0;
}
