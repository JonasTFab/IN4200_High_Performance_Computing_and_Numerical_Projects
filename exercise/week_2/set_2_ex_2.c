#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>



void task_a()
{

  int n = 100000;
  int a[n];
  int b1[n];
  int b2[n];

  printf("Length of vector to be copied: %d\n", n);

  // declaration of vector a
  for (int i=0; i<n; i++)
  {
    a[i] = i;
  }


  time_t t0 = clock();
  // copy vector a by using for loop
  for (int i=0; i<n; i++)
  {
    b1[i] = a[i];
  }
  time_t t_loop = clock() - t0;
  printf("Time used using for loop: %f s\n", ((double)t_loop)/CLOCKS_PER_SEC);



  t0 = clock();
  // copy vector a by using memcpy
  memcpy(b2, a, sizeof(int)*n);
  time_t t_memcpy = clock() - t0;
  printf("Time used using memcpy: %f s\n", ((double)t_memcpy)/CLOCKS_PER_SEC);

  printf("Time usage ratio of for loop/memcpy: %f\n", ((double)t_loop/(double)t_memcpy));


  // checks if b1 == b2
  for (int i=0; i<n; i++)
  {
    if (b1[i] != b2[i])
    {
      printf("b1 not equal to b2!\n");
      exit(-1);
    }
  }

}






int main()
{

  printf("Task a:\n");
  task_a();


  return 0;
}
