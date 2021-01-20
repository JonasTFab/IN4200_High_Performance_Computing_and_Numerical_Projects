#include <stdio.h>
#include <stdlib.h>


void swap(int *a, int *b)
{
  int t=*a;
  *a=*b;
  *b=t;
}


void sort(int arr[], int perm[], int beg, int end)
{

  if (end > beg + 1)
  {
    int piv = arr[beg];
    int l = beg + 1;
    int r = end;
    while (l < r)
    {
      if (arr[l] <= piv)
        l++;
      else
        swap(&arr[l], &arr[--r]);
        swap(&perm[l], &perm[--r]);

    }
    swap(&arr[--l], &arr[beg]);
    swap(&perm[--l], &perm[beg]);
    sort(arr, perm, beg, l);
    sort(arr, perm, r, end);
  }

}






void task_a()
{
  int len_arr = 10;
  int array[len_arr];
  int perm[len_arr];
  time_t t;       // to randomize the rand() function each runtime
  srand(t);

  // defining array of length 'len_arr' with random numbers
  for (int i=0; i<len_arr; i++)
  {
    array[i] = rand() % 10;
    perm[i] = i;
    printf("array[%d] = %d    perm[%d] = %d\n", i, array[i], i, perm[i]);
  }

  // perm does'nt work, don't know why
  sort(array, perm, 0, len_arr);

  printf("\nSorted\n");
  for (int i=0; i<len_arr; i++)
  {
    printf("array[%d] = %d    perm[%d] = %d\n", i, array[i], i, perm[i]);
  }


}










int main()
{

  printf("Task a:\n");
  task_a();


  return 0;
}
