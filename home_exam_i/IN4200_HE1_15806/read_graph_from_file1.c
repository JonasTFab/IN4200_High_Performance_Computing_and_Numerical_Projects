#include <stdio.h>
#include <stdlib.h>


void read_graph_from_file1(char *filename, int *N, char ***table2D)
{
  FILE *file;
  int len_line = 200;
  char read_line[len_line];
  int edges;

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
  fscanf(file, "%s %s %d %s %d", skip, skip, N, skip, &edges);
  fgets(read_line, len_line, file);  // jumps to next line



  // allocates the 2D table in memory
  *table2D = (char**)malloc(*N*sizeof(char*));
  for (int i=0; i<*N; i++)
  {
    (*table2D)[i] = (char*)malloc(*N*sizeof(char));
    for (int j=0; j<*N; j++)
    {
      (*table2D)[i][j] = 0;   // initialise all values to 0
    }
  }



  // read connections between two nodes from file
  int int1, int2;
  int tot_edges = edges;
  for (int i=0; i<edges; i++)
  {
    fgets(read_line, len_line, file);
    fscanf(file, "%d %d", &int1, &int2);

    // checking if undirected edge is unique
    if ((*table2D)[int1][int2]==1 || (*table2D)[int2][int1]==1)
    {
      tot_edges--;
    }

    // checking if edge is self-link and also nodes are
    // not 'illegal', i.e. lower than zero and greater
    // than the total number of nodes
    if (int1 != int2 && int1<*N && int2<*N && int1>-1 && int2>-1)
    {
      (*table2D)[int1][int2] = 1;
      (*table2D)[int2][int1] = 1;
    }
    else
    {
      tot_edges--;
    }

  }

  fclose(file);

  if (edges != tot_edges)
  {
    printf("Warning!\n There were %d illegal edges!", edges-tot_edges);
  }

}





int main()
{
  char *con_file = "connectivity_graph.txt";
  //char *con_file = "simple-graph.txt";
  int nodes;
  char **table;

  // reading the text file and allocates the number of
  // nodes and
  read_graph_from_file1(con_file, &nodes, &table);


  printf("Nodes:   %d\n", nodes);
  printf("Printing 2D table: \n");
  for (int i=0; i<nodes; i++)
  {
    for (int j=0; j<nodes; j++)
    {
      printf("%d  ", table[i][j]);
    }
    printf("\n");
  }


  free(table);
  free(*table);



  return 0;
}
