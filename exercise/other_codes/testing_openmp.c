#include <stdio.h>
#include <omp.h>

int main()
{

  #pragma omp parallel
  {
    int thread, tot_threads;
    tot_threads = omp_get_num_threads();
    thread = omp_get_thread_num();
    printf("Thread %d out of %d\n", thread+1, tot_threads);
  }

  return 0;
}
