#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*
allocates the 3D array u and v
*/
double ***task_a(double ***arr, int x, int y, int z)
{

  // allocates a 3D array
  printf("Allocate array... ");
  arr = (double***)malloc(x*sizeof(double**));
  for (int i=0; i<x; i++)
  {
    arr[i] = (double**)malloc(x*y*sizeof(double*));
    for (int j=0; j<y; j++)
    {
      arr[i][j] = (double*)malloc(x*y*z*sizeof(double));
    }
  }
  printf("Done!\n");

  return arr;
}



/*
initialise values in the v array that follows some expression
*/
double ***task_b(double ***arr, int x, int y, int z)
{
  double arg;

  printf("Initialize values in array... ");
  for (int i=0; i<x; i++)
  {
    for (int j=0; j<y; j++)
    {
      for (int k=0; k<z; k++)
      {
        arg = (i*j*k*M_PI) / ((x-1)*(y-1)*(z-1));
        // sin() does not work (fixed by including -lm when compiling)
        arr[i][j][k] = 2.0 + sin(arg);
      }
    }
  }
  printf("Done!\n");

  return arr;

}



/*
initialise values in the u array with zeros
*/
double ***task_c(double ***arr, int x, int y, int z)
{

  printf("Initialize values in array... ");
  for (int i=0; i<x; i++)
  {
    for (int j=0; j<y; j++)
    {
      for (int k=0; k<z; k++)
      {
      arr[i][j][k] = 0;
      }
    }
  }
  printf("Done!\n");

  return arr;
}



/*
calculates the new values that is to be replaced in the v array
*/
double ***task_d(double ***arr1, double ***arr2, int x, int y, int z)
{

  printf("Calculating values in array... \n");
  for (int i=1; i<x-1; i++)
  {
    for (int j=1; j<y-1; j++)
    {
      for (int k=1; k<z-1; k++)
      {
      arr1[i][j][k] = arr2[i][j][k] + (arr2[i-1][j][k] + arr2[i][j-1][k] +
                      arr2[i][j][k-1] - 6*arr2[i][j][k] + arr2[i+1][j][k] +
                      arr2[i][j+1][k] + arr2[i][j][k+1]) / 6;
      printf("u[%d][%d][%d] = %f     v[%d][%d][%d] = %f\n", \
              i, j, k, arr1[i][j][k], i, j, k, arr2[i][j][k]);
      }
    }
  }
  printf("Done!\n");

  return arr1;
}









int main()
{
  int nx = 10;      // first dimension
  int ny = 5;       // second dimension
  int nz = 3;       // third dimension

  double ***u;      // define pointer to 3D array
  double ***v;      // define pointer to 3D array


  printf("Task a:\n");
  u = task_a(u, nx, ny, nz);
  v = task_a(v, nx, ny, nz);


  printf("\nTask b:\n");
  v = task_b(v, nx, ny, nz);


  printf("\nTask c:\n");
  u = task_c(u, nx, ny, nz);


  printf("\nTask d:\n");
  u = task_d(u, v, nx, ny, nz);


  printf("\nTask e:\n");
  printf("Deallocating u and v... ");
  free(u);
  free(v);
  printf("Done!\n");


  return 0;
}
