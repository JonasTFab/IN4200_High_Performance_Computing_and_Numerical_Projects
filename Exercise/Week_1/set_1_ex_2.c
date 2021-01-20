#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void task_a()
{
  int len_time = 5;
  int len_temp = 10;

  FILE *file_pointer;
  file_pointer = fopen("daily_temperature.txt", "r");

  char single_line[50];
  char time[len_time];
  char temp[len_temp];

  double temperature;
  double max_temp = -100;
  double min_temp = 100;
  char max_temp_time[len_time];
  char min_temp_time[len_time];


  // feof = file end of file
  // ! = not
  // 'keep looping till end of file'
  while (!feof(file_pointer))
  {
    fgets(single_line, 250, file_pointer);    // gets a line from file


    for (int i=0; i<len_time; i++)
    {
      time[i] = single_line[i];
    }

    temperature = atof(&single_line[len_time]);


    if (temperature > max_temp)
    {
      for (int i=0; i<len_time; i++)
      {
        max_temp_time[i] = time[i];
      }
      max_temp = temperature;
    }

    if (temperature < min_temp)
    {
      for (int i=0; i<len_time; i++)
      {
        min_temp_time[i] = time[i];
      }
      min_temp = temperature;
    }
  }

  fclose(file_pointer);


  printf("Max temperature (%.1f C) at %s\n", max_temp, max_temp_time);
  printf("Min temperature (%.1f C) at %s\n", min_temp, min_temp_time);

}



void smooth(int **v_new, int **v, int nx, int ny, double c)
{

  /*
  Extension of the 'smooth' function to be applicable for a 2D array
  */

  for (int i=1; i<nx-1; i++)
  {
    for (int j=1; j<ny-1; j++)
    {
      v_new[i][j] = v[i][j] + c*(v[i-1][j] + v[i][j-1] - 4*v[i][j]
                                + v[i][j+1] + v[i+1][j]);
    }
  }

  for (int i=0; i<nx; i+=nx-1)
  {
    for (int j=0; j<ny; j++)
    {
      v_new[i][j] = v[i][j];
    }
  }

  for (int i=0; i<nx; i++)
  {
    for (int j=0; j<ny; j+=ny-1)
    {
      v_new[i][j] = v[i][j];
    }
  }
}



void task_b()
{
  int m = 3;
  int n = 4;
  //int arr[m][n];
  double constant = 0.5;
  int max_ran = 10;       // highest possible random value
  time_t t;       // to randomize the rand() function each runtime
  srand(t);


  int **arr = (int**)malloc(m*sizeof(int*));
  for (int i=0; i<m; i++)
  {
    arr[i] = (int*)malloc(n*sizeof(int));
  }


  for (int i=0; i<m; i++)
  {
    for (int j=0; j<n; j++)
    {
      arr[i][j] = rand() % max_ran + 1;
    }
  }

  // new array
  int **new_arr = (int**)malloc(m*sizeof(int*));
  for (int i=0; i<m; i++)
  {
    new_arr[i] = (int*)malloc(n*sizeof(int));
  }

  // smoothing function
  smooth(new_arr, arr, m, n, constant);


  // print the results
  for (int i=0; i<m; i++)
  {
    for (int j=0; j<n; j++)
    {
      printf("arr[%d][%d] / new_arr[%d][%d] = %d / %d\n", i, j, i, j, arr[i][j], new_arr[i][j]);
    }
  }

  free(arr);
  free(new_arr);
}






int main()
{

  printf("Task a:\n");
  task_a();

  printf("\n\nTask b:\n");
  task_b();



  return 0;
}
