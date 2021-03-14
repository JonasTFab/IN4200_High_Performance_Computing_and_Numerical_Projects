#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


  // arrays used for counting occurance of
  // each node and connections to each node.
  // This method is used so that we do not need to
  // read the text file multiple times but only once
  int *count_nodes;
  char **count_connections;
  count_nodes = (int*)malloc(*N*sizeof(int));
  count_connections = (char**)malloc(*N*sizeof(char*));
  for (int i=0; i<*N; i++)
  {
    count_connections[i] = (char*)malloc(500);
  }


  // read connectivity pair from text file
  int int1, int2;
  char num[10];
  int tot_edges = N_edges;
  for (int row=0; row<N_edges; row++)
  {
    fgets(read_line, len_line, file);
    fscanf(file, "%d %d", &int1, &int2);

    // check of legal values
    if (int1<0 || int1>=*N || int2<0 || int2>=*N)
    {
      printf("Illegal edge in file (line %d+5):        (%d, %d)\n", row, int1, int2);
      tot_edges--;
    }

    // check of self-links
    else if (int1 == int2)
    {
      printf("Illegal self-link in file (line %d+5):   (%d, %d)\n", row, int1, int2);
      tot_edges--;
    }

    else
    {
      count_nodes[int1]++;
      count_nodes[int2]++;

      sprintf(num, "%d ", int2);
      strcat(count_connections[int1], num);

      sprintf(num, "%d ", int1);
      strcat(count_connections[int2], num);
    }
  }


  // allocates row_ptr and col_idx as 1D arrays (in a CRS format)
  *row_ptr = (int*)malloc((*N+1)*sizeof(int));     // defined length of row_ptr
  *col_idx = (int*)malloc(2*tot_edges*sizeof(int));  // defined length of col_idx

  int idx = 0;
  for (int node=0; node<*N; node++)
  {
    // insert integer in row pointer
    (*row_ptr)[node+1] = (*row_ptr)[node] + count_nodes[node];

    char num[10];     // resets the 'num' paremeter
    // reads the counted connection and insert the integers in col_idx array
    for (int i = 0; count_connections[node][i] != '\0'; i++)
    {
      if (i==0 || count_connections[node][i] != ' ' && count_connections[node][i-1] == ' ')
      {
        int1 = atoi(&count_connections[node][i]);
        sprintf(num, "%d ", int1);
      }

      else if (count_connections[node][i] == ' ' && count_connections[node][i-1] != ' ')
      {
        int2 = atoi(num);
        (*col_idx)[idx] = int2;

        idx++;
        char num[10];

      }
    }
  }


  if (tot_edges != N_edges)
  {
    printf("Legal edges:   %d/%d\n", tot_edges, N_edges);
  }

  free(count_nodes);
  free(count_connections);
  fclose(file);

}



int main()
{
  char *con_file = "connectivity_graph.txt";
  //char *con_file = "combined.txt";
  //char *con_file = "simple-graph.txt";
  int nodes;
  int *row_pointer, *column_index;

  // read a text file and from it, ...
  //
  read_graph_from_file2(con_file, &nodes, &row_pointer, &column_index);


  // deallocate arrays from memory
  free(row_pointer);
  free(column_index);

  return 0;
}
