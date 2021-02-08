#include <stdio.h>
#include <stdlib.h>         // enables 'rand()' function
#include <math.h>           // enables 'fabs()' function


void task_a()
{
  int i, sign;
  float out, denum, eps, true_val;
  out = 1;
  denum = 1;
  true_val = 4.0/5;
  eps = 1e-5;


  i = 1;
  while (fabs(out-true_val) > eps)
  {
    // setting correct sign
    if (i%2 == 0)
    {
      sign = 1;
    }

    else
    {
      sign = -1;
    }

    denum = denum*4;

    out = out + sign * 1 / (denum);
    i++;

  }
  printf("Obtained approximately 4/5 after %d iterations. Result: %f \n\n\n", i-1, out);
}


void task_b()
{
  int min, max, max_arr;
  time_t t;       // to randomize the rand() function each runtime
  srand(t);

  int len_arr = 10;
  int array[len_arr];
  max_arr = 100;

  min = max_arr;
  max = 0;

  for (int i=0; i<len_arr; i++)
  {
    // assigning new element value to the array
    array[i] = rand() % max_arr;

    // checks if new element in array is a minimum
    if (array[i] < min)
    {
      min = array[i];
    }

    // checks if new element in array is a maximum
    if (array[i] > max)
    {
      max = array[i];
    }

    printf("array[%d]:  %d\n", i, array[i]);

  }

  printf("\nMin value in the array: %d \n", min);
  printf("Max value in the array: %d \n\n\n", max);
}


void task_c()
{
  int m = 3;
  int n = 4;
  int matrix[m][n];

  for (int i=0; i<m; i++)
  {
    for (int j=0; j<n; j++)
    {
      matrix[i][j] = i*m + j*n;
    }
  }

  for (int i=0; i<m; i++)
  {
    for (int j=0; j<n; j++)
    {
      printf("matrix[%d][%d]: %d\n", i, j, matrix[i][j]);
    }
  }

  printf("\n");

  for (int j=0; j<n; j++)
  {
    for (int i=0; i<m; i++)
    {
      printf("matrix[%d][%d]: %d\n", i, j, matrix[i][j]);
    }
  }

  printf("\nWe note that the order of the for loops does not matter!\n\n\n");


}


void task_d()
{
  int nx = 3;
  int ny = 4;
  int nz = 2;
  int ***u;

  u = (int***)malloc(nx*sizeof(int**));

  for (int i=0; i<nx; i++)
  {
    u[i] = (int**)malloc(ny*sizeof(int*));
    for (int j=0; j<ny; j++)
    {
      u[i][j] = (int*)malloc(nz*sizeof(int));
    }
  }

  for (int i=0; i<nx; i++)
  {
    for (int j=0; j<ny; j++)
    {
      for (int k=0; k<nz; k++)
      {
        u[i][j][k] = i*ny*nz + j*nz*k;
        printf("u[%d][%d][%d] = ", i,j,k);
        printf("%d*%d*%d + %d*%d + %d = ", i,ny,nz,j,nz,k);
        printf("%d\n", u[i][j][k]);
      }
    }
  }
  free(u);


}



void main()
{
  //  First exercise in IN4200

  printf("Task a:\n");
  task_a();

  printf("Task b:\n");
  task_b();

  printf("Task c:\n");
  task_c();

  printf("Task d:\n");
  task_d();



}
