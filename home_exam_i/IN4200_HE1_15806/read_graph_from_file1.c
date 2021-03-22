
void read_graph_from_file1(char *filename, int *N, char ***table2D)
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


  // allocates the symmetric 2D table in memory. since
  // the table is allocated as 'char', we do not need
  // to initialise every element with 0
  *table2D = (char**)malloc(*N*sizeof(char*));
  for (int i=0; i<*N; i++)
  {
    (*table2D)[i] = (char*)malloc(*N*sizeof(char));
    for (int j=0; j<*N; j++)
    {
      (*table2D)[i][j] = 0;   // initiate each element in table with 0
    }
  }


  // read connections between two nodes from file
  int int1, int2;
  int tot_edges = N_edges;
  for (int i=0; i<N_edges; i++)
  {
    fgets(read_line, len_line, file);
    fscanf(file, "%d %d", &int1, &int2);

    // check of legal edges
    if (int1<0 || int1>=*N || int2<0 || int2>=*N)
    {
      printf("Illegal edge in file (line %d+5):        (%d, %d)\n", i, int1, int2);
      tot_edges--;
    }

    // check of self-links
    else if (int1 == int2)
    {
      printf("Illegal self-link in file (line %d+5):   (%d, %d)\n", i, int1, int2);
      tot_edges--;
    }

    // insert the legal edge to the table
    else
    {
      (*table2D)[int1][int2] = 1;
      (*table2D)[int2][int1] = 1;
    }
  } // end of outer loop


  // close the file from memory
  fclose(file);


  // just to print if illegal edges were found in file
  if (N_edges != tot_edges)
  {
    printf("Legal edges:   %d/%d\n", tot_edges, N_edges);
  }


  // printing the full 2D table (for troubleshooting)
  /*
  printf("Nodes:   %d\n", *N);
  printf("Printing 2D table: \n");
  for (int i=0; i<*N; i++)
  {
    for (int j=0; j<*N; j++)
    {
      printf("%d  ", (*table2D)[i][j]);
    }
    printf("\n");
  }
  */

} // end of void function
