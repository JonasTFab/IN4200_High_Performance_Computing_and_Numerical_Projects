
void check_node (int node_id, int tau, int N, int *row_ptr, int *col_idx,
                 int *SNN_val)
{
  // first we check if the input node is a valid check
  if (node_id<0 || node_id>=N)
  {
    printf("The input node id (%d) is not valid!\n", node_id);
    printf("Please enter a node id between 0 and %d.\n", N-1);
    printf("Program killed!\n");
    exit(0);
  }

  // allocate an array for storing possible clustered nodes
  char *cluster;
  cluster = (char*)malloc(N*sizeof(char));
  for (int i=0; i<N; i++)
  {
    cluster[i] = -1;  // -1 since 0 are a node by definition
  }
  cluster[node_id] = 0;   // 0 means node to be checked for SNN


  int i,j, check_nodes=1, tot_nodes=0;
  while (check_nodes==1)
  {
    check_nodes = 0;
    // iterates over every nodes
    for (i=0; i<N; i++)
    {
      // if an element with 0 is found,
      // we investigate this furhter
      if (cluster[i]==0)
      {
        // node is checked and does'nt need to be checked again
        cluster[i] = 1;
        for (j=row_ptr[i]; j<row_ptr[i+1]; j++)
        {
          if (SNN_val[j] >= tau && cluster[col_idx[j]] == -1)
          {
            cluster[col_idx[j]] = 0;
            check_nodes = 1;      // check nodes again
          }
        } // end of inner for loop
      } // end of outer if
    } // end of outer for loop
  } // end of while loop

  cluster[node_id] = -1;   // exclude the initial node from cluster


  printf("Node %d in cluster with ", node_id);
  printf("(cluster threshold=%d):\nNode(s): ", tau);
  for (int i=0; i<N; i++)
  {
    if (cluster[i] != -1)
    {
      printf("%d ", i);
      tot_nodes++;
    }
  }
  if (tot_nodes != 0) tot_nodes++;
  printf("\nThe cluster consists of a total of %d node(s).\n", tot_nodes);

  // deallocate array
  free(cluster);

} // end of void function
