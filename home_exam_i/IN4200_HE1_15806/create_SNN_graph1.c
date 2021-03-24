
void create_SNN_graph1(int N, char **table2D, int ***SNN_table)
{

  // allocate the symmetric 2D SNN table
  *SNN_table = (int**)malloc(N*sizeof(int*));
  for (int i=0; i<N; i++)
  {
    (*SNN_table)[i] = (int*)malloc(N*sizeof(int));
    for (int j=0; j<N; j++)
    {
      (*SNN_table)[i][j] = 0;
    }
  }


  int i,j,k;
  // parallelization is implemented as it fits for files
  // with many nodes and edges. dynamic is used since each
  // iteration in outer loop has not the same length.
  #pragma omp parallel for private(i,j,k) schedule(dynamic)
    for (i=0; i<N; i++)
    {
      // from j=i+1 as we need only to check half of the matrix (symmetric)
      // +1 since the diagonal are always zero ( [i,j]=0 )
      for (j=i+1; j<N; j++)
      {
        for (k=0; k<N; k++)
        {
          // first state: checks that two nodes are connected to each other
          // second and third statement: excluding pairwise nodes
          // fourth and fifth statement: checks that both nodes are connected
          //                             to a shared node
          if (table2D[i][j]==1 && k!=i && k!=j && table2D[i][k]==1 && table2D[j][k]==1)
          {
            (*SNN_table)[i][j]++;
            (*SNN_table)[j][i]++;
          }
        } // end of inner loop 2
      } // end of inner loop 1
    } // end of outer loop




  /*
  // printing the SNN table (for troubleshooting)
  for (int i=0; i<N; i++)
  {
    for (int j=0; j<N; j++)
    {
      printf("%d ", (*SNN_table)[i][j]);
    } // end of inner loop
    printf("\n");
  } // end of outer loop
  */


} // end of void function
