#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include "6-2_2018.c"



void odd_even_block_paralell (int m, int *sublist)
{

  int my_rank, procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);

  for (int i=0; i<=procs; i++)
  {
    //printf("rank: %d,   i: %d,   i modulo 2: %d \n", my_rank, i, i%2);
    if (i%2 == 1)
    {
      if (my_rank%2 == 0 && my_rank<procs-1)
      {
        compare_split(m, sublist, my_rank, my_rank+1);
      }
      else if (my_rank%2 == 1 && my_rank>0)
      {
        compare_split(m, sublist, my_rank, my_rank-1);
      }
    }

    else
    {
      if (my_rank%2 == 1 && my_rank<procs-1)
      {
        compare_split(m, sublist, my_rank, my_rank+1);
      }
      else if (my_rank%2 == 0 && my_rank>0)
      {
        compare_split(m, sublist, my_rank, my_rank-1);
      }
    }
  }

}







int main()
{
  int my_rank, procs, m=5;

  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);

  int *list;
  list = (int*)malloc(m*sizeof(int));

  //sleep(my_rank);


  for (int i=0; i<m; i++)
  {
    list[i] = 50 - m*(my_rank+1) + i*(my_rank+2);
    printf("rank: %d   list[%d]: %d \n", my_rank, i, list[i]);
  }

  odd_even_block_paralell(m, list);
  //sleep(3-my_rank);

  if (my_rank==procs-1) printf("\n\n\n");

  //sleep(procs + my_rank+1);
  sleep(1);

  for (int i=0; i<m; i++)
  {
    printf("rank: %d   list[%d]: %d \n", my_rank, i, list[i]);
  }

  free(list);
  MPI_Finalize();

  return 0;
}
