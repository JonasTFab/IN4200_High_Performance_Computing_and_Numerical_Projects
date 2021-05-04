

void MPI_double_layer_convolution(int M, int N, float **input,
                                  int K1, float **kernel1,
                                  int K2, float **kernel2,
                                  float **output)
{

  int my_rank, procs, *counts=NULL, *displacement=NULL;
  float **temp_arr=NULL;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);


  // gets the number of rows each process should have (to
  // distribute the work as equally as possible)
  //int row_low = my_rank*M/procs;        // lower indexed row
  //int row_high = (my_rank+1)*M/procs;   // higher indexed row
  int row_low = my_rank*(M-K1+1)/procs;        // lower indexed row
  int row_high = (my_rank+1)*(M-K1+1)/procs;   // higher indexed row
  int rank_rows = row_high - row_low;   // total number of rows per rank

  // the last K1-1 rows is removed in the 'temp_arr' array,
  // hence not used for calculation. Therefore, the last
  // rank should have K1-1 extra rows to distribute the work
  // equally among the processors
  if (my_rank==procs-1)
  {
    rank_rows += K1-1;
  }


  // allocate a 1D underlying contiguous array 'input_rank'
  // array used for partinioning of 'input' array
  float **input_rank = malloc(rank_rows*sizeof(input_rank));
  input_rank[0] = malloc(rank_rows*N*sizeof(float));
  for (int i=1; i<rank_rows; i++)
  {
    input_rank[i] = &(input_rank[0][i*N]);
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
    for (int i=0; i<(M-K1+1); i++)
    {
      for (int j=0; j<(N-K1+1); j++)
      {
        temp_arr[i][j] = 0;
      }
    }



    // allocate the number of elements sent to the ranks as
    // 'counts' array. The starting element relative to
    // the 'input' array is stored in the 'displacement' array
    counts = malloc(procs*sizeof(int));
    displacement = malloc(procs*sizeof(int));

    counts[0] = ((M-K1+1)/procs)*N;
    displacement[0] = 0;

    for (int i=1; i<procs; i++)
    {
      counts[i] = ((i+1)*(M-K1+1)/procs - i*(M-K1+1)/procs)*N;
      displacement[i] = displacement[i-1] + counts[i-1];
    }
    counts[procs-1] = counts[procs-1] + (K1-1)*N;


    // scatters the 'input' array to all proceccors
    MPI_Scatterv(&input[0][0],      // const void *sendbuf
                 counts,       // const int *sendcounts
                 displacement,      // const int *displs
                 MPI_FLOAT,         // MPI_Datatype sendtype
                 &input_rank[0][0],   // void *recvbuf
                 rank_rows*N,       // int recvcount
                 MPI_FLOAT,         // MPI_Datatype recvtype
                 0,                 // int root
                 MPI_COMM_WORLD);   // MPI_Comm comm

  } // end of 'if (my_rank==0)' block

  // recieve the scattered 'input' array to other
  // ranks than the root
  else
  {
    MPI_Scatterv(NULL,              // const void *sendbuf
                 NULL,              // const int *sendcounts
                 NULL,              // const int *displs
                 MPI_FLOAT,         // MPI_Datatype sendtype
                 &input_rank[0][0],   // void *recvbuf
                 rank_rows*N,       // int recvcount
                 MPI_FLOAT,         // MPI_Datatype recvtype
                 0,                 // int root
                 MPI_COMM_WORLD);   // MPI_Comm comm


    if (my_rank==procs-1)
    {
      rank_rows -= K1-1;
    }

    // array to fill values after first convolutional process
    temp_arr = malloc(rank_rows*sizeof(temp_arr));
    temp_arr[0] = malloc(rank_rows*(N-K1+1)*sizeof(float));
    for (int i=1; i<rank_rows; i++)
    {
      temp_arr[i] = &(temp_arr[0][i*(N-K1+1)]);
    }


    // fill the temporary array with zeros
    for (int i=0; i<rank_rows; i++)
    {
      for (int j=0; j<(N-K1+1); j++)
      {
        temp_arr[i][j] = 0;
      }
    }




  } // end of 'else' block

  //MPI_Barrier(MPI_COMM_WORLD);



  // do more stuff from here
  //....




  float **send_arr = malloc((K1-1)*sizeof(send_arr));
  float **recv_arr = malloc((K1-1)*sizeof(recv_arr));
  send_arr[0] = malloc((K1-1)*(N-K1+1)*sizeof(float));
  recv_arr[0] = malloc((K1-1)*(N-K1+1)*sizeof(float));
  for (int i=1; i<(K1-1); i++)
  {
    send_arr[i] = &(send_arr[0][i*(N-K1+1)]);
    recv_arr[i] = &(recv_arr[0][i*(N-K1+1)]);
  }

  for (int i=0; i<K1-1; i++)
  {
    for (int j=0; j<N-K1+1; j++)
    {
      send_arr[i][j] = 0;
      recv_arr[i][j] = 0;
    }
  }



  int row, k, l, kk, ll;
  char send, recv;
  float element_value;
  // convolutional multiplication on first layer
  for (k=0; k<rank_rows; k++)
  {

    if (k<K1-1 && my_rank!=0)
    {
      //send = K1 - k - 1;
      // fill 'send' array with values
      for (int i=0; i<k+1; i++)
      {
        for (int j=0; j<N-K1+1; j++)
        {
          for (ll=0; ll<K1; ll++)
          {
            send_arr[k][j] += input_rank[i][j+ll] * kernel1[K1-1-k+i][ll];
          }
        }
      }
    } // end of if block



    for (l=0; l<N-K1+1; l++)
    {
      element_value = 0;
      //temp_arr[k][l] = 0;
      for (kk=0; kk<K1; kk++)
      {

        // making sure not to exceed 'input_rank' array limit
        if (kk < rank_rows-k)
        {
          for (ll=0; ll<K1; ll++)
          {
            temp_arr[k][l] += input_rank[k+kk][l+ll] * kernel1[kk][ll];
            //element_value += input_rank[k+kk][l+ll] * kernel1[kk][ll];
          }
        } // end of if block

        // last processor will not exclude 'input_rank' limit
        else if (my_rank==procs-1)
        {
          for (ll=0; ll<K1; ll++)
          {
            temp_arr[k][l] += input_rank[k+kk][l+ll] * kernel1[kk][ll];
            //element_value += input_rank[k+kk][l+ll] * kernel1[kk][ll];
          }
        } // end of else block
      } // end of second inner loop
    } // end of first inner loop
  } // end of outer for loop


  // send info to one rank lower
  if (my_rank!=0)
  {
    //printf("rank %d is sending!\n", my_rank);
    MPI_Send(&send_arr[0][0],
             (K1-1)*(N-K1+1),
             MPI_FLOAT,
             my_rank-1,
             my_rank,
             MPI_COMM_WORLD);
  } // end of 'if rank != 0' block

  // recieve info from one rank higher
  if (my_rank!=procs-1)
  {
    //printf("rank %d is recieving!\n", my_rank);
    MPI_Recv(&recv_arr[0][0],
             (K1-1)*(N-K1+1),
             MPI_FLOAT,
             my_rank+1,
             my_rank+1,
             MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    for (int i=0; i<K1-1; i++)
    {
      for (int j=0; j<N-K1+1; j++)
      {
        temp_arr[rank_rows-K1+1+i][j] += recv_arr[i][j];
      }
    }
  } // end of 'if rank != last' block




  int row_low2 = my_rank*(M-K1-K2+2)/procs;        // lower indexed row
  int row_high2 = (my_rank+1)*(M-K1-K2+2)/procs;   // higher indexed row
  int rank_rows2 = row_high2 - row_low2;   // total number of rows per rank

  // the last K1+K2-2 rows is removed in the 'output' array,
  // hence not used for calculation. Therefore, the last
  // rank should have K1+K2-2 extra rows to distribute the work
  // equally among the processors

  if (my_rank==procs-1)
  {
    rank_rows2 += K2-1;
  }

  int row_send = row_high-row_high2;
  int row_recv = row_low-row_low2;


  // reallocate 'input_rank' array
  input_rank = malloc(rank_rows2*sizeof(input_rank));
  input_rank[0] = malloc(rank_rows2*(N-K1+1)*sizeof(float));
  for (int i=1; i<rank_rows2; i++)
  {
    input_rank[i] = &(input_rank[0][i*(N-K1+1)]);
  }
  for (int i=0; i<rank_rows2; i++)
  {
    for (int j=0; j<N-K1+1; j++)
    {
      input_rank[i][j] = 0;
    }
  }


  if (row_send && my_rank!=procs-1)
  {
    MPI_Send(&temp_arr[rank_rows-row_send][0],
             (row_send)*(N-K1+1),
             MPI_FLOAT,
             my_rank+1,
             my_rank,
             MPI_COMM_WORLD);
  }

  if (row_recv)
  {
    MPI_Recv(&input_rank[0][0],
             (row_recv)*(N-K1+1),
             MPI_FLOAT,
             my_rank-1,
             my_rank-1,
             MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  }


  for (int i=row_recv; i<rank_rows2; i++)
  {
    for (int j=0; j<N-K1+1; j++)
    {
      input_rank[i][j] = temp_arr[i-row_recv][j];
    }
  }

  if (my_rank==procs-1)
  {
    rank_rows2 -= K2-1;
  }

  // allocate a partitioning 'output' array
  float **output_rank = malloc(rank_rows2*sizeof(output_rank));
  output_rank[0] = malloc(rank_rows2*(N-K1-K2+2)*sizeof(float));
  for (int i=1; i<rank_rows2; i++)
  {
    output_rank[i] = &(output_rank[0][i*(N-K1-K2+2)]);
  }

  for (int i=0; i<rank_rows2; i++)
  {
    for (int j=0; j<N-K1-K2+2; j++)
    {
      output_rank[i][j] = 0;
    }
  }


  // reallocate 'send_arr' and 'recv_arr'
  send_arr = malloc((K2-1)*sizeof(send_arr));
  recv_arr = malloc((K2-1)*sizeof(recv_arr));
  send_arr[0] = malloc((K2-1)*(N-K1-K2+2)*sizeof(float));
  recv_arr[0] = malloc((K2-1)*(N-K1-K2+2)*sizeof(float));
  for (int i=1; i<(K2-1); i++)
  {
    send_arr[i] = &(send_arr[0][i*(N-K1-K2+2)]);
    recv_arr[i] = &(recv_arr[0][i*(N-K1-K2+2)]);
  }

  for (int i=0; i<K2-1; i++)
  {
    for (int j=0; j<N-K1-K2+2; j++)
    {
      send_arr[i][j] = 0;
      recv_arr[i][j] = 0;
    }
  }



  // convolutional multiplication on second layer
  for (k=0; k<rank_rows2; k++)
  {

    if (k<K2-1 && my_rank!=0)
    {
      // fill 'send' array with values
      for (int i=0; i<k+1; i++)
      {
        for (int j=0; j<N-K1-K2+2; j++)
        {
          for (ll=0; ll<K2; ll++)
          {
            send_arr[k][j] += input_rank[i][j+ll] * kernel2[K2-1-k+i][ll];
          }
        }
      }
    } // end of if block


    for (l=0; l<N-K1-K2+2; l++)
    {
      element_value = 0;
      //temp_arr[k][l] = 0;
      for (kk=0; kk<K2; kk++)
      {
        // making sure not to exceed 'input_rank' array limit
        if (kk < rank_rows2-k)
        {
          for (ll=0; ll<K2; ll++)
          {
            output_rank[k][l] += input_rank[k+kk][l+ll] * kernel2[kk][ll];
          }
        } // end of if block

        else if (my_rank==procs-1)
        {
          for (ll=0; ll<K2; ll++)
          {
            output_rank[k][l] += input_rank[k+kk][l+ll] * kernel2[kk][ll];
          }
        } // end of else block
      } // end of second inner loop
    } // end of first inner loop
  } // end of outer for loop


  // send info to one rank lower
  if (my_rank!=0)
  {
    //printf("rank %d is sending!\n", my_rank);
    MPI_Send(&send_arr[0][0],
             (K2-1)*(N-K1-K2+2),
             MPI_FLOAT,
             my_rank-1,
             my_rank,
             MPI_COMM_WORLD);
  } // end of 'if rank != 0' block

  // recieve info from one rank higher
  if (my_rank!=procs-1)
  {
    //printf("rank %d is recieving!\n", my_rank);
    MPI_Recv(&recv_arr[0][0],
             (K2-1)*(N-K1-K2+2),
             MPI_FLOAT,
             my_rank+1,
             my_rank+1,
             MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);


    for (int i=0; i<K2-1; i++)
    {
      for (int j=0; j<N-K1-K2+2; j++)
      {
        output_rank[rank_rows2-K2+1+i][j] += recv_arr[i][j];
      }
    }


  } // end of 'if rank != last' block


  // final gatherv...
  // gather the 'output_rank' arrays into root rank
  if (my_rank!=0)
  {
    MPI_Gatherv(&output_rank[0][0],      // const void *sendbuf
                rank_rows2*(N-K1-K2+2),   // const int *sendcounts
                MPI_FLOAT,            // MPI_Datatype sendtype
                NULL,                 // void *recvbuf
                NULL,                 // int recvcount
                NULL,                 // const int *displs
                MPI_FLOAT,            // MPI_Datatype recvtype
                0,                    // int root
                MPI_COMM_WORLD);      // MPI_Comm comm
  }



  if (my_rank==0)
  {

    counts[0] = (M-K1-K2+2)/procs*(N-K1-K2+2);
    for (int i=1; i<procs; i++)
    {
      counts[i] = ((i+1)*(M-K1-K2+2)/procs - i*(M-K1-K2+2)/procs)*(N-K1-K2+2);
      displacement[i] = displacement[i-1] + counts[i-1];
    }


    MPI_Gatherv(&output_rank[0][0],                 // const void *sendbuf
                rank_rows2*(N-K1-K2+2),           // const int *sendcounts
                MPI_FLOAT,            // MPI_Datatype sendtype
                &output[0][0],      // void *recvbuf
                counts,          // int recvcount
                displacement,         // const int *displs
                MPI_FLOAT,            // MPI_Datatype recvtype
                0,                    // int root
                MPI_COMM_WORLD);      // MPI_Comm comm

  } // end of 'if rank == 0' block





  free(counts);
  free(displacement);
  free(input_rank);
  free(temp_arr);
  free(send_arr);
  free(recv_arr);
  free(output_rank);



} // end of function
