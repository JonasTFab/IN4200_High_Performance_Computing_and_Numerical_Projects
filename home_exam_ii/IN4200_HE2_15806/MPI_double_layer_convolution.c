

void MPI_double_layer_convolution(int M, int N, float **input,
                                  int K1, float **kernel1,
                                  int K2, float **kernel2,
                                  float **output)
{

  int my_rank, procs, *send_counts=NULL, *displacement=NULL;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);


  if (my_rank==0)
  {
    send_counts = (int*)malloc(procs*sizeof(int));
    displacement = (int*)malloc(procs*sizeof(int));

    for (int i=0; i<procs; i++)
    {
      send_counts[i] = ((i+1)*M/procs - i*M/procs)*N;
      displacement[i] = i*M/procs;
    }
  } // end of 'if (my_rank==0)' block


  int rank_group_size = (my_rank+1)*M/procs - my_rank*M/procs;
  printf("My rank %d got %d/%d rows\n", my_rank, rank_group_size, M);
  float **rank_buf = (float**)malloc(rank_group_size*sizeof(float*));
  for (int i=0; i<rank_group_size; i++)
  {
    rank_buf[i] = (float*)malloc(N*sizeof(float));
  }



  //MPI_Barrier(MPI_COMM_WORLD);
  MPI_Scatterv(&input[0][0],             //DONE const void *sendbuf
               send_counts,       //DONE const int *sendcounts
               displacement,      //DONE const int *displs
               MPI_FLOAT,         //DONE MPI_Datatype sendtype
               rank_buf,          //DONE void *recvbuf
               send_counts[my_rank], //rank_group_size*N, //DONE int recvcount
               MPI_FLOAT,         //DONE MPI_Datatype recvtype
               0,                 //DONE int root
               MPI_COMM_WORLD);   //DONE MPI_Comm comm



  if (my_rank==1)
  {
    for (int i=0; i<rank_group_size; i++)
    {
      for (int j=0; j<N; j++)
      {
        printf("%f  ", rank_buf[i][j]);
        }
      printf("\n");
    }
  }






  free(send_counts);
  free(displacement);
  free(rank_buf);


} // end of function
