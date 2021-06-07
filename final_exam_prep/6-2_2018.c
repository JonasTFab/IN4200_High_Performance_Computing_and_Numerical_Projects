#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include "6-1_2018.c"



void compare_split (int m, int *my_sublist, int my_MPI_rank, int other_MPI_rank)
{
  int *list_recv, *merged_list;

  list_recv = (int*)malloc(m*sizeof(int));
  merged_list = (int*)malloc(2*m*sizeof(int));

  for (int i=0; i<2*m; i++)
  {
    merged_list[i] = 0;
    if (i<m)
    {
      list_recv[i] = 0;
    }
  }

  MPI_Send( my_sublist,
            m,
            MPI_INT,
            other_MPI_rank,
            0,
            MPI_COMM_WORLD);

  MPI_Recv( list_recv,
            m,
            MPI_INT,
            other_MPI_rank,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);


  merge(m, my_sublist, list_recv, merged_list);


  for (int i=0; i<m; i++)
  {
    my_sublist[i] = merged_list[i+m*my_MPI_rank];
    //printf("I'm rank %d with list[%d]=%d \n", my_MPI_rank, i, my_sublist[i]);
  }

  free(list_recv);
  free(merged_list);

}





/*
int main()
{
  int *list, n=5, my_rank, dest, other_rank;


  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  list = (int*)malloc(n*sizeof(int));
  for (int i=0; i<n; i++)
  {
    list[i] = 0;
  }


  // give 'list' values
  if (my_rank==0)
  {
    list[0]=1; list[1]=6; list[2]=9; list[3]=13; list[4]=14;
    other_rank = 1;
  }
  // give 'list' values
  else
  {
    list[0]=4; list[1]=5; list[2]=10; list[3]=16; list[4]=19;
    other_rank = 0;
  }





  compare_split(n, list, my_rank, other_rank);

  free(list);
  MPI_Finalize();

  return 0;
}

*/
