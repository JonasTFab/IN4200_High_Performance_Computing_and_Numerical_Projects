

void MPI_double_layer_convolution(int M, int N, float **input,
                                  int K1, float **kernel1,
                                  int K2, float **kernel2,
                                  float **output)
{

  int my_rank, procs, *send_counts=NULL, *displacement=NULL, **temp_arr=NULL;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);


  // gets the number of rows each process should have (to
  // distribute the work as equally as possible)
  int row_low = my_rank*M/procs;        // lower indexed row
  int row_high = (my_rank+1)*M/procs;   // higher indexed row
  int rank_rows = row_high - row_low;   // total number of rows per rank

  // allocate a 1D underlying contiguous array 'rank_buf'
  // array used for partinioning of 'input' array
  float **rank_buf = malloc(rank_rows*sizeof(rank_buf));
  rank_buf[0] = malloc(rank_rows*N*sizeof(float));
  for (int i=1; i<rank_rows; i++)
  {
    rank_buf[i] = &(rank_buf[0][i*N]);
  }



  // when in root rank, send information from 'input' to all ranks
  if (my_rank==0)
  {
    // allocate a temporary array as output after first
    // convolution in a 1D underlying contiguous approach
    temp_arr = malloc((M-K1+1)*sizeof(temp_arr));
    temp_arr[0] = malloc((M-K1+1)*(N-K1+1)*sizeof(float));
    for (int i=1; i<(M-K1+1); i++)
    {
      temp_arr[i] = &(temp_arr[0][i*(N-K1+1)]);
    }

    // fill the temporary array with zeros
    for (int i=1; i<(M-K1+1); i++)
    {
      for (int j=1; j<(N-K1+1); j++)
      {
        temp_arr[i][j] = 0;
      }
    }


    // allocate the number of elements sent to the ranks as
    // 'send_counts' array. The starting element relative to
    // the 'input' array is stored in the 'displacement' array
    send_counts = malloc(procs*sizeof(int));
    displacement = malloc(procs*sizeof(int));

    send_counts[0] = ((M-K1+1)/procs)*N;
    displacement[0] = 0;

    for (int i=1; i<procs; i++)
    {
      send_counts[i] = ((i+1)*(M-K1+1)/procs - i*(M-K1+1)/procs)*N;
      displacement[i] = displacement[i-1] + send_counts[i-1];
    }
    send_counts[procs] = send_counts[procs] + (K1-1)/procs*N;

    // scatters the 'input' array to all proceccors
    MPI_Scatterv(&input[0][0],      // const void *sendbuf
                 send_counts,       // const int *sendcounts
                 displacement,      // const int *displs
                 MPI_FLOAT,         // MPI_Datatype sendtype
                 &rank_buf[0][0],   // void *recvbuf
                 rank_rows*N,       // int recvcount
                 MPI_FLOAT,         // MPI_Datatype recvtype
                 0,                 // int root
                 MPI_COMM_WORLD);   // MPI_Comm comm

  } // end of 'if (my_rank==0)' block


  else
  {
    MPI_Scatterv(NULL,              // const void *sendbuf
                 NULL,              // const int *sendcounts
                 NULL,              // const int *displs
                 MPI_FLOAT,         // MPI_Datatype sendtype
                 &rank_buf[0][0],   // void *recvbuf
                 rank_rows*N,       // int recvcount
                 MPI_FLOAT,         // MPI_Datatype recvtype
                 0,                 // int root
                 MPI_COMM_WORLD);   // MPI_Comm comm
  } // end of 'else' block

  MPI_Barrier(MPI_COMM_WORLD);



  // do more stuff from here
  //....

  /*
  // last K1+1 columns should not be calculated
  if (my_rank==procs-1)
  {
    rank_rows -= K1-1;
  }
  */


  float **fill_arr = malloc(2*sizeof(send_arr));
  send_arr[0] = malloc(2*(N-K1+1)*sizeof(float));
  for (int i=1; i<2; i++)
  {
    send_arr[i] = &(send_arr[0][i*(N-K1+1)]);
  }



  float **send_arr = malloc(2*sizeof(send_arr));
  send_arr[0] = malloc(2*(N-K1+1)*sizeof(float));
  for (int i=1; i<2; i++)
  {
    send_arr[i] = &(send_arr[0][i*(N-K1+1)]);
  }

  float **recv_arr = malloc(2*sizeof(recv_arr));
  recv_arr[0] = malloc(2*(N-K1+1)*sizeof(float));
  for (int i=1; i<2; i++)
  {
    recv_arr[i] = &(recv_arr[0][i*(N-K1+1)]);
  }


  int row, k, l, kk, l;
  char send, recv;
  float element_value;
  // convolutional multiplication on first layer
  for (k=0; k<rank_rows; k++)
  {
    send = 0;
    recv = 0;

    row = row_low + k;
    //printf("rank: %d,  local row: %d,   global row: %d\n", my_rank, k, row);
    if (k+K1 > rank_rows)
    {
      recv = k + K1 - rank_rows;
      printf("RECIEVE  k: %d,  k+K1: %d,  rank_rows: %d,  recieve: %d \n", k, k+K1, rank_rows, recv);
    }

    //if (my_rank!=0 && k<K1 && (k+1)%K1!=0)
    if (k<K1-1 && my_rank!=0)
    {
      send = K1 - k - 1;
      printf("SEND  k: %d,  rank: %d,  send: %d \n", k, my_rank, send);
    }

    for (l=0; l<N-K1+1; l++)
    {
      element_value = 0;
      for (kk=0; kk<K1; kk++)
      {
        if (kk < rank_rows-k)
        {
          for (ll=0; ll<K1; ll++)
          {
            element_value += rank_buf[k+kk][l+ll] * kernel1[kk][ll];
          }
        } // end of if block
        else
        {

        }
      } // end of second inner loop





      /*
      if (my_rank==0)
      {
        MPI_Gatherv(&element_value,      // const void *sendbuf
                    1,                   // const int *sendcounts
                    MPI_FLOAT,           // MPI_Datatype sendtype
                    &temp_arr,     // void *recvbuf
                    1,                   // int recvcount
                    row*N+l,               // const int *displs
                    MPI_FLOAT,           // MPI_Datatype recvtype
                    0,                   // int root
                    MPI_COMM_WORLD);     // MPI_Comm comm
      }

      else2
      {
        MPI_Gatherv(&element_value,      // const void *sendbuf
                    1,                   // const int *sendcounts
                    MPI_FLOAT,           // MPI_Datatype sendtype
                    NULL,//temp_arr[k][l],     // void *recvbuf
                    1,                   // int recvcount
                    row*N,               // const int *displs
                    MPI_FLOAT,           // MPI_Datatype recvtype
                    0,                   // int root
                    MPI_COMM_WORLD);     // MPI_Comm comm
      }
      */

    } // end of first inner loop
  } // end of outer for loop





  free(send_counts);
  free(displacement);
  free(rank_buf);
  free(temp_arr);
  free(send_arr);
  free(recv_arr);


} // end of function
