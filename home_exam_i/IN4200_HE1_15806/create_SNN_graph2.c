
void create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val)
{
  // before we allocate the SNN_val array, we need to
  // know the number of edges which can be found by the
  // last element in row_ptr. SNN_val has the same length
  // as col_idx.
  int len_col_idx = row_ptr[N];     // equals 2*N_edges
  *SNN_val = (int*)malloc(len_col_idx*sizeof(int));
  for (int i=0; i<len_col_idx; i++)
  {
    (*SNN_val)[i] = 0;
  }



  int i,j,k,h;
  // a big mess
  // we do not need to check for self-links or illegal edges
  // as this is done when reading the text file. the following
  // for loop is parallelized dynamically as each iteration
  // has not the same length.
  #pragma omp parallel for private(i,j,k,h) schedule(dynamic)
    for (i=0; i<N; i++)
    {
      for (j=row_ptr[i]; j<row_ptr[i+1]; j++)
      {
        for (k=row_ptr[i]; k<row_ptr[i+1]; k++)
        {
          if (col_idx[j]!=col_idx[k])
          {
            for (h=row_ptr[col_idx[j]]; h<row_ptr[col_idx[j]+1]; h++)
            {
              if (col_idx[k]==col_idx[h])
              {
                (*SNN_val)[j]++;
                break;  // if match, we skip the rest
              }
            } // end of inner loop 3
          } // end of if statement
        } // end of inner loop 2
      } // end of inner loop 1
    } // end of outer loop



  // printing the row_ptr, col_idx and SNN_val (for troubleshooting)
  /*
  printf("row_ptr(node):   col_idx(SNN)\n");
  for (int i=0; i<N; i++)
  {
    printf("%d(%d):        ", row_ptr[i], i);

    for (int j=row_ptr[i]; j<row_ptr[i+1]; j++)
    {
      printf("%d(%d)  ", col_idx[j], (*SNN_val)[j]);
    }
    printf("\n");
  }
  */

} // end of void function
