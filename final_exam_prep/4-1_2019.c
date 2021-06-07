#include <mpi.h>
#include <stdio.h>

int main (int nargs, char **args)
{
  int own_value, in_value, out_value, i;
  int rank, size;
  int send_to, recv_from;

  MPI_Status recv_status;
  MPI_Request recv_req;

  MPI_Init (&nargs, &args);
  MPI_Comm_size (MPI_COMM_WORLD, &size);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  own_value = rank;
  recv_from = (rank+2)%size;
  send_to = (rank-2+size)%size;
  out_value = own_value;

  for (i=0; i<(size/2)-1; i++)
  {
    MPI_Irecv(&in_value, 1, MPI_INT, recv_from, 0, MPI_COMM_WORLD, &recv_req);
    MPI_Send (&out_value, 1, MPI_INT, send_to, 0, MPI_COMM_WORLD);
    MPI_Wait (&recv_req, &recv_status);
    own_value += in_value;
    out_value = in_value;
  }

  printf("On rank <%d>, own_value=%d, recv_from=%d, send_to=%d \n",rank,own_value,recv_from,send_to);
  MPI_Finalize ();
  return 0;
}
