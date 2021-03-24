
void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx)
{
  FILE *file;
  int len_line = 200;
  char read_line[len_line];
  int N_edges;

  // open text file for reading
  file = fopen(filename, "r");

  // skip the first two rows in text file
  for (int i=0; i<2; i++)
  {
    fgets(read_line, len_line, file);
  }

  // reads the third line in text file and define number
  // of nodes and number of edges
  char skip[20];
  fscanf(file, "%s %s %d %s %d", skip, skip, N, skip, &N_edges);
  fgets(read_line, len_line, file);  // jumps to next line


  // Arrays used for counting occurance of each
  // node and add all connections to each node.
  // This method is used so that we do not need to
  // read the text file multiple times but only once.
  int *count_nodes;
  char **count_connections;
  count_nodes = (int*)malloc(*N*sizeof(int));
  count_connections = (char**)malloc(*N*sizeof(char*));
  for (int i=0; i<*N; i++)
  {
    count_nodes[i] = 0;
    // size of 10*N to make sure all possible nodes are included
    count_connections[i] = (char*)malloc(*N*10);
  }


  // read edges from text file
  int node1, node2;
  char num[10];
  int tot_edges = N_edges;    // to count illegal edges
  for (int row=0; row<N_edges; row++)
  {
    // read one line
    fgets(read_line, len_line, file);
    fscanf(file, "%d %d", &node1, &node2);

    // check of legal edges
    if (node1<0 || node1>=*N || node2<0 || node2>=*N)
    {
      printf("Illegal edge in file (line %d+5):        (%d, %d)\n", row, node1, node2);
      tot_edges--;
    }

    // check of self-links
    else if (node1 == node2)
    {
      printf("Illegal self-link in file (line %d+5):   (%d, %d)\n", row, node1, node2);
      tot_edges--;
    }

    // legal edges are counted and added to the proivisional arrays
    else
    {
      count_nodes[node1]++;
      count_nodes[node2]++;

      sprintf(num, "%d ", node2);      // convert int to string
      strcat(count_connections[node1], num); // add string to array

      sprintf(num, "%d ", node1);      // convert int to string
      strcat(count_connections[node2], num); // add string to array
    }
  } // end of loop


  // allocates row_ptr and col_idx as 1D arrays (in CRS format)
  *row_ptr = (int*)malloc((*N+1)*sizeof(int));      // defined length of row_ptr
  *col_idx = (int*)malloc(2*tot_edges*sizeof(int)); // defined length of col_idx

  int idx = 0;
  for (int node=0; node<*N; node++)
  {
    // insert number of connections in row pointer
    (*row_ptr)[node+1] = (*row_ptr)[node] + count_nodes[node];

    char num[10];     // resets the 'num' paremeter
    // reads the count_connections array for each node and
    // insert the connected nodes, if any, in col_idx array
    for (int i = 0; count_connections[node][i] != '\0'; i++)
    {
      if (i==0 || count_connections[node][i] != ' ' && count_connections[node][i-1] == ' ')
      {
        node1 = atoi(&count_connections[node][i]);
        sprintf(num, "%d ", node1);
      }

      else if (count_connections[node][i] == ' ' && count_connections[node][i-1] != ' ')
      {
        node2 = atoi(num);
        (*col_idx)[idx] = node2;

        idx++;
        char num[10];
      } // end of else if
    } // end of inner loop
  } // end of outer loop


  // deallocate the proivisional arrays and the file
  free(count_nodes);
  free(count_connections);
  fclose(file);


  // just to print if illegal edges were found in file
  if (tot_edges != N_edges)
  {
    printf("Legal edges:   %d/%d\n", tot_edges, N_edges);
  }


  // printing the row_ptr and col_idx(for troubleshooting)
  /*
  printf("row_ptr:   col_idx\n");
  for (int i=0; i<*N; i++)
  {
    printf("%d:    ", (*row_ptr)[i]);
    for (int j=(*row_ptr)[i]; j<(*row_ptr)[i+1]; j++)
    {
      printf("%d ", (*col_idx)[j]);
    }
    printf("\n");
  }
  */

} // end of void function
