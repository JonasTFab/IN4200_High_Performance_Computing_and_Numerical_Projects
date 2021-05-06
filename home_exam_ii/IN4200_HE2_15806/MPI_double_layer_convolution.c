
/*
This program is made to perform a convolutional computation
on a 2D array, typically on a grayscaled image. Hence, a
2D 'input' array should be sent as an argument as well as its
dimension 'M' and 'N'. 'M' and 'N' defines the number of rows and
columns in the input array respectively. When performing the
convolutional computation on the 'input' array, two additional
filter kernels are required, namely 'kernel1' and 'kernel2'.
Both are quadratric arrays with dimension 'K1' and 'K2'. The
dimension of the kernels are two required inputs as well.
The last input is the 'output', which is the resulted array
after a double convolutional computation and has dimension
(M-K1-K2+2) * (N-K1-K2+2) in the end.

The program utilize MPI for performance boost. It does so
by splitting the amount of rows 'M' from the input array into
equal sized partitions and send them to the desired number of
processors, also known as ranks. Each rank is then given the
commands to perform the convolution on their respective partition.
*/


void MPI_double_layer_convolution(int M, int N, float **input,
                                  int K1, float **kernel1,
                                  int K2, float **kernel2,
                                  float **output)
{
  // Initiate some parameters.
  // my_rank: the rank of the current processors
  // procs: total number of processors used
  // counts: array[procs], each element in 'counts' represents
  //         how many values each rank will send or recieve
  //         through MPI_Scatterv or MPI_Gatherv
  // displacement: array[procs], each element indicate
  //               the displacement relative to the sending
  //               or receiving array in MPI_Scatterv or MPI_Gatherv
  // temp_arr: temporary array, contain results values after
  //           first convolutional layer
  int i, j, my_rank, procs, *counts=NULL, *displacement=NULL;
  float **temp_arr=NULL, **send_arr, **recv_arr, **output_rank;
  int M1 = M-K1+1;      // number of rows after first convolution
  int M2 = M-K1-K2+2;   // number of rows after second convolution
  int N1 = N-K1+1;      // number of columns after first convolution
  int N2 = N-K1-K2+2;   // number of columns after second convolution

  // Gets processor rank and the total number of processors currentty using.
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);


  // Division of work with respect to the number of rows is used.
  // Gets the number of rows each process should have (to
  // distribute the work as equally as possible). To distribute the
  // work equally, we multiply by M1 instead of M as the last
  // K1-1 rows is excluded in output array
  int row_low = my_rank*M1/procs;           // lower indexed row
  int row_high = (my_rank+1)*M1/procs;      // higher indexed row
  int rank_rows = row_high - row_low;       // total number of rows per rank

  // The last K1-1 rows is removed in the output 'temp_arr' array,
  // hence not used for calculation. Therefore, the last
  // rank should have K1-1 extra rows to distribute the work
  // equally among processors.
  if (my_rank==procs-1)
  {
    rank_rows += K1-1;
  }



  // Allocate a 1D underlying contiguous array 'input_rank'
  // array used for partitioning of 'input' array
  float **input_rank = malloc(rank_rows*sizeof(input_rank));
  input_rank[0] = malloc(rank_rows*N*sizeof(float));
  for (i=1; i<rank_rows; i++)
  {
    input_rank[i] = &(input_rank[0][i*N]);
  }



  // When in root, distribute parts of 'input' array to all ranks,
  // including root
  if (my_rank==0)
  {
    // Allocate a temporary array as output after first
    // convolution computation, as a 1D underlying contiguous array
    temp_arr = malloc(M1*sizeof(temp_arr));
    temp_arr[0] = malloc(M1*N1*sizeof(float));
    for (i=1; i<M1; i++)
    {
      temp_arr[i] = &(temp_arr[0][i*N1]);
    }

    // Fill the temporary array with zeros
    for (i=0; i<M1; i++)
    {
      for (j=0; j<N1; j++)
      {
        temp_arr[i][j] = 0;
      }
    }


    // Allocate the number of elements sent to each rank as
    // 'counts' array. The starting index relative to the
    // contiguously 1D 'input' array is stored in 'displacement'
    counts = malloc(procs*sizeof(int));
    displacement = malloc(procs*sizeof(int));

    counts[0] = row_high*N;   // # of rows * # of columns
    displacement[0] = 0;      // first index should not be displaced

    for (i=1; i<procs; i++)
    {
      counts[i] = ((i+1)*M1/procs - i*M1/procs)*N;
      displacement[i] = displacement[i-1] + counts[i-1];
    }
    // Last rank will recieve M1*N extra elements
    counts[procs-1] = counts[procs-1] + (K1-1)*N;


    // Scatters the 'input' array to all proceccors
    MPI_Scatterv(&input[0][0],      // array to scatter
                 counts,            // # of elements to send to each rank
                 displacement,      // displacement relative to the scattered array
                 MPI_FLOAT,         // type of data sent
                 &input_rank[0][0], // array to recieve incoming data
                 rank_rows*N,       // # of elements receiving
                 MPI_FLOAT,         // type of data received
                 0,                 // root
                 MPI_COMM_WORLD);   // communicator

  } // End of 'if (my_rank==0)' block



  // Recieve the scattered 'input' array to the other
  // ranks but the root
  else
  {
    MPI_Scatterv(NULL,              // unnecessary when receiving data
                 NULL,              // unnecessary when receiving data
                 NULL,              // unnecessary when receiving data
                 MPI_FLOAT,         // type of data sent
                 &input_rank[0][0], // array to recieve incoming data
                 rank_rows*N,       // # of elements receiving
                 MPI_FLOAT,         // type of data received
                 0,                 // root
                 MPI_COMM_WORLD);   // communicator


    // Reduce the number of rows in last rank by K1-1
    if (my_rank==procs-1)
    {
      rank_rows -= K1-1;
    }

    // Allocate the temporary 'temp_arr' array as 1D contiguous array
    // uniquely in each rank
    temp_arr = malloc(rank_rows*sizeof(temp_arr));
    temp_arr[0] = malloc(rank_rows*N1*sizeof(float));
    for (i=1; i<rank_rows; i++)
    {
      temp_arr[i] = &(temp_arr[0][i*N1]);
    }

    // Fill the temporary array with zeros
    for (i=0; i<rank_rows; i++)
    {
      for (j=0; j<N1; j++)
      {
        temp_arr[i][j] = 0;
      }
    }


  } // end of 'else' block (my_rank != 0)




  // Allocate two additional arrays namely 'send_arr' and 'recv_arr'.
  // As we perform the convolutional computation with K1*K1 and K2*K2
  // kernels, we will eventually exceed the number of rows each rank has.
  // Therefore, we need to send K1-1 rows worth of computation from the
  // one-level higher rank to the one-lever lower rank. Both arrays is
  // allocated as a 1D contiguously array
  send_arr = malloc((K1-1)*sizeof(send_arr));
  recv_arr = malloc((K1-1)*sizeof(recv_arr));
  send_arr[0] = malloc((K1-1)*N1*sizeof(float));
  recv_arr[0] = malloc((K1-1)*N1*sizeof(float));
  for (i=1; i<(K1-1); i++)
  {
    send_arr[i] = &(send_arr[0][i*N1]);
    recv_arr[i] = &(recv_arr[0][i*N1]);
  }

  // Fill the 'send_arr' and 'recv_arr' arrays with zeros
  for (i=0; i<K1-1; i++)
  {
    for (j=0; j<N1; j++)
    {
      send_arr[i][j] = 0;
      recv_arr[i][j] = 0;
    }
  }



  int k, l, kk, ll;   // Pre-initiate parameters used in multiple loops
  // First layer of convolutional computation. Each rank has a given
  // number of rows 'rank_ranws' they will use for calculation.
  for (k=0; k<rank_rows; k++)
  {

    // The first K1-1 rows (not rank 0) must send calculation to
    // the one-level rank lower. Each row in 'send_arr' is computed
    // such that the rows should be added to the K1-1 lowest rows
    // in the one-level lower rank. e.g. the first row in 'send_arr'
    // contains values such as 'input_rank[0]*kernel1[K1-1]', second
    // row gets the values 'input_rank[0]*kernel1[K1-2] +
    // input_rank[1]*kernel1[K1-1]' and so on... Note that only the
    // row number is used in the example above
    if (k<K1-1 && my_rank!=0)
    {
      // Loop through rows lower than current row 'k'
      for (i=0; i<k+1; i++)
      {
        for (j=0; j<N1; j++)
        {
          for (ll=0; ll<K1; ll++)
          {
            // Fill 'send_arr' with values
            send_arr[k][j] += input_rank[i][j+ll] * kernel1[K1-1-k+i][ll];
          }
        } // End of first inner loop
      } // End of outer for loop
    } // End of if 'k<K1-1' block


    // For each row, loop over every element in row
    for (l=0; l<N1; l++)
    {
      // Loop over rows in 'kernel1'
      for (kk=0; kk<K1; kk++)
      {
        // Making sure not to exceed 'input_rank' array limit
        if (kk<rank_rows-k)
        {
          // Loop over columns in 'kernel1'
          for (ll=0; ll<K1; ll++)
          {
            // Fill temporary array with values
            temp_arr[k][l] += input_rank[k+kk][l+ll] * kernel1[kk][ll];
          }
        } // End of if 'kk<rank_rows-k' block

        // Last processor will not exclude 'input_rank' limit
        else if (my_rank==procs-1)
        {
          for (ll=0; ll<K1; ll++)
          {
            temp_arr[k][l] += input_rank[k+kk][l+ll] * kernel1[kk][ll];
          }
        } // End of else if 'my_rank==procs-1' block
      } // End of second inner loop
    } // End of first inner loop
  } // End of outer for loop


  // Ranks not equal to zero will send rows to
  // one-level rank lower with MPI
  if (my_rank!=0)
  {
    MPI_Send(&send_arr[0][0],     // array to send
             (K1-1)*N1,           // number of elements to send
             MPI_FLOAT,           // type of elements
             my_rank-1,           // to which rank
             my_rank,             // tag
             MPI_COMM_WORLD);     // communicator
  } // End of 'if rank != 0' block

  // Ranks (not last rank) will receive rows from
  // one-level rank higher with MPI
  if (my_rank!=procs-1)
  {
    MPI_Recv(&recv_arr[0][0],     // array to receive data
             (K1-1)*N1,           // number of elements to receive
             MPI_FLOAT,           // type of elements
             my_rank+1,           // from which rank
             my_rank+1,           // tag
             MPI_COMM_WORLD,      // communicator
             MPI_STATUS_IGNORE);  // status

    // The last K1-1 rows will be filled with
    // the missing values
    for (i=0; i<K1-1; i++)
    {
      for (j=0; j<N1; j++)
      {
        temp_arr[rank_rows-K1+1+i][j] += recv_arr[i][j];
      }
    }
  } // End of 'if rank != last' block




  // First layer of convolution is done at this point.
  // From here we perform a second convolution, very
  // much with the same approach but by using a second
  // kernel, namely 'kernel2' that has 'K2' rows and columns



  // Division of rows to each rank
  int row_low2 = my_rank*M2/procs;         // lower indexed row
  int row_high2 = (my_rank+1)*M2/procs;    // higher indexed row
  int rank_rows2 = row_high2 - row_low2;   // total number of rows per rank

  // the last K1+K2-2 rows is removed in the 'output' array,
  // hence not used for calculation. Therefore, the last
  // rank should have K1+K2-2 extra rows to distribute the work
  // equally among the processors
  if (my_rank==procs-1)
  {
    rank_rows2 += K2-1;
  }

  // We could just use the 'temp_arr' when performing
  // the next convolution, but then the work among
  // processors would not be equally. The last rank would
  // have K1-1 rows less of computation. We would then shift
  // some of the rows over to the next rank so that the last
  // rank gets K2-1 extra rows
  int row_send = row_high-row_high2;  // number of rows to shift
  int row_recv = row_low-row_low2;    // number of rows to receive


  // Reallocate 'input_rank' array contiguously
  free(input_rank);
  input_rank = malloc(rank_rows2*sizeof(input_rank));
  input_rank[0] = malloc(rank_rows2*N1*sizeof(float));
  for (i=1; i<rank_rows2; i++)
  {
    input_rank[i] = &(input_rank[0][i*N1]);
  }
  // Fill 'input_rank' with zeros
  for (i=0; i<rank_rows2; i++)
  {
    for (j=0; j<N1; j++)
    {
      input_rank[i][j] = 0;
    }
  }


  // if 'row_send != 0', this rank should shift some
  // of the rows to the one-level higher rank
  if (row_send && my_rank!=procs-1)
  {
    MPI_Send(&temp_arr[rank_rows-row_send][0],  // array to send
             (row_send)*N1,       // number of elements to send
             MPI_FLOAT,           // type of elements
             my_rank+1,           // to which rank
             my_rank,             // tag
             MPI_COMM_WORLD);     // communicator
  }

  // 'my_rank' receive the shifted rows from
  // one-level rank lower
  if (row_recv)
  {
    MPI_Recv(&input_rank[0][0],   // array to receive data
             (row_recv)*N1,       // number of elements to receive
             MPI_FLOAT,           // type of elements
             my_rank-1,           // from which rank
             my_rank-1,           // tag
             MPI_COMM_WORLD,      // communicator
             MPI_STATUS_IGNORE);  // status
  }

  // Apply the shifted rows (if any) to the 'input_rank' array
  for (i=row_recv; i<rank_rows2; i++)
  {
    for (j=0; j<N1; j++)
    {
      input_rank[i][j] = temp_arr[i-row_recv][j];
    }
  }

  // Redure the rows on the last rank by K1-1 as these rows
  // is not needed for further calculation
  if (my_rank==procs-1)
  {
    rank_rows2 -= K2-1;
  }


  // Allocate a partitioning 'output_rank' array that later is
  // send to the final 'output' array
  output_rank = malloc(rank_rows2*sizeof(output_rank));
  output_rank[0] = malloc(rank_rows2*N2*sizeof(float));
  for (i=1; i<rank_rows2; i++)
  {
    output_rank[i] = &(output_rank[0][i*N2]);
  }

  // Fill 'output_rank' arrays with zeros
  for (i=0; i<rank_rows2; i++)
  {
    for (j=0; j<N2; j++)
    {
      output_rank[i][j] = 0;
    }
  }


  // Reallocate 'send_arr' and 'recv_arr' contiguously.
  // These array serves the same purpose as in the first
  // convolution but with other dimensions
  free(send_arr);
  free(recv_arr);
  send_arr = malloc((K2-1)*sizeof(send_arr));
  recv_arr = malloc((K2-1)*sizeof(recv_arr));
  send_arr[0] = malloc((K2-1)*N2*sizeof(float));
  recv_arr[0] = malloc((K2-1)*N2*sizeof(float));
  for (i=1; i<K2-1; i++)
  {
    send_arr[i] = &(send_arr[0][i*N2]);
    recv_arr[i] = &(recv_arr[0][i*N2]);
  }

  // Fill 'send_arr' and 'recv_arr' will zeros
  for (i=0; i<K2-1; i++)
  {
    for (j=0; j<N2; j++)
    {
      send_arr[i][j] = 0;
      recv_arr[i][j] = 0;
    }
  }


  // Second layer of convolution
  for (k=0; k<rank_rows2; k++)
  {
    // First K2-1 rows (not in rank 0) will send computation
    // to the one-level rank lower
    if (k<K2-1 && my_rank!=0)
    {
      // Loop through rows lower than current row 'k'
      for (i=0; i<k+1; i++)
      {
        for (j=0; j<N2; j++)
        {
          for (ll=0; ll<K2; ll++)
          {
            // Fill 'send_arr' with values
            send_arr[k][j] += input_rank[i][j+ll] * kernel2[K2-1-k+i][ll];
          }
        } // End of first inner loop
      } // End of outer for loop
    } // End of if 'k<K2-1' block


    // Loop over every element in a row
    for (l=0; l<N2; l++)
    {
      for (kk=0; kk<K2; kk++)
      {
        // Making sure not to exceed 'input_rank' array limit
        if (kk < rank_rows2-k)
        {
          for (ll=0; ll<K2; ll++)
          {
            // Fill 'output_rank' with values
            output_rank[k][l] += input_rank[k+kk][l+ll] * kernel2[kk][ll];
          }
        } // End of if 'kk<rank_rows2-k' block

        // Last processor will not exclude 'input_rank' limit
        else if (my_rank==procs-1)
        {
          for (ll=0; ll<K2; ll++)
          {
            output_rank[k][l] += input_rank[k+kk][l+ll] * kernel2[kk][ll];
          }
        } // End of else block
      } // End of second inner loop
    } // End of first inner loop
  } // End of outer for loop


  // Ranks not equal to zero will send rows to
  // one-level rank lower with MPI
  if (my_rank!=0)
  {
    MPI_Send(&send_arr[0][0],     // array to send
             (K2-1)*N2,           // number of elements to send
             MPI_FLOAT,           // type of elements
             my_rank-1,           // to which rank
             my_rank,             // tag
             MPI_COMM_WORLD);     // communicator
  } // End of 'if rank != 0' block

  // Ranks (not last rank) will receive rows from
  // one-level rank higher with MPI
  if (my_rank!=procs-1)
  {
    MPI_Recv(&recv_arr[0][0],     // array to receive data
             (K2-1)*N2,           // number of elements to receive
             MPI_FLOAT,           // type of elements
             my_rank+1,           // from which rank
             my_rank+1,           // tag
             MPI_COMM_WORLD,      // communicator
             MPI_STATUS_IGNORE);  // status



    // The last K2-1 rows will be filled with
    // the missing values
    for (i=0; i<K2-1; i++)
    {
      for (j=0; j<N2; j++)
      {
        output_rank[rank_rows2-K2+1+i][j] += recv_arr[i][j];
      }
    }

  } // End of 'if rank != last' block


  // Finally, all values in 'output_rank' from all ranks should
  // be filled in the 'output' array located in rank 0 (root).
  // This is done using the 'MPI_Gatherv'. All non-root ranks
  // send their respective arrays to the root
  if (my_rank!=0)
  {
    MPI_Gatherv(&output_rank[0][0],   // array to send to root
                rank_rows2*N2,        // number of elements to send
                MPI_FLOAT,            // type of elements sent
                NULL,                 // unnecessary when sending data
                NULL,                 // unnecessary when sending data
                NULL,                 // unnecessary when sending data
                MPI_FLOAT,            // type of element received
                0,                    // root
                MPI_COMM_WORLD);      // communicator
  }


  // Root rank gather data from other ranks, including from itself
  if (my_rank==0)
  {
    // Refill 'counts' and 'displacement' array with values.
    counts[0] = row_high2*N2;   // # of rows * # of columns in rank 0
    for (i=1; i<procs; i++)
    {
      counts[i] = ((i+1)*M2/procs - i*M2/procs)*N2;       // # of rows * # of columns
      displacement[i] = displacement[i-1] + counts[i-1];  // displacement relative to the 'output' array
    }


    MPI_Gatherv(&output_rank[0][0],   // array to send to root
                rank_rows2*N2,        // number of elements to send
                MPI_FLOAT,            // type of elements sent
                &output[0][0],        // array to store the received elements
                counts,               // # of elements received from each rank
                displacement,         // displacement relative to the receiving array
                MPI_FLOAT,            // type of element received
                0,                    // root
                MPI_COMM_WORLD);      // communicator

  } // End of 'if rank == 0' block




  // Deallocate all arrays defined in
  // 'MPI_double_layer_convolution.c' function
  free(counts);
  free(displacement);
  free(input_rank);
  free(temp_arr);
  free(send_arr);
  free(recv_arr);
  free(output_rank);



} // end of function
