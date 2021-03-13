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


  // allocates row_ptr and col_idx as 1D arrays (in a CRS format)
  *row_ptr = (int*)malloc((*N+1)*sizeof(int));     // defined length of row_ptr
  *col_idx = (int*)malloc(2*N_edges*sizeof(int));  // defined length of col_idx


  // count occurance of each node and connections to each node
  int *count_nodes;
  char **count_connections;
  count_nodes = (int*)malloc(*N*sizeof(int));
  count_connections = (char**)malloc(*N*sizeof(char*));
  for (int i=0; i<*N; i++)
  {
    count_nodes[i] = 0;
    count_connections[i] = (char*)malloc(50);
  }



  // read connectivity pair from text file
  int int1, int2;
  char num[10];
  int tot_edges = N_edges;
  for (int row=0; row<N_edges; row++)
  {
    fgets(read_line, len_line, file);
    fscanf(file, "%d %d", &int1, &int2);

    count_nodes[int1]++;
    count_nodes[int2]++;

    sprintf(num, "%d ", int2);
    strcat(count_connections[int1], num);

    sprintf(num, "%d ", int1);
    strcat(count_connections[int2], num);
  }


  // NOTE! Just for printing, not necessary
  for (int i=0; i<*N; i++)
  {
    printf("%d(%d):   %s\n", count_nodes[i], i, count_connections[i]);
  }



  /*
  for (int i = 0; count_connections[2][i] != '\0'; i++)
  {
    printf("%s ", &count_connections[2][i]);

    //if (&count_connections[2][i] != " ")
    if (count_connections[2][i] != ' ' && count_connections[2][i+1] == ' ')
    {
      int1 = atoi(&count_connections[2][i]);
      printf("-- %d", int1);
    }
    printf("\n");
    //printf("%s \n" , &count_connections[2][i]);
  }
  */


  printf("----------\n");

  int idx = 0;
  for (int node=0; node<*N; node++)
  {
    (*row_ptr)[node+1] = (*row_ptr)[node] + count_nodes[node];
    //for (int con=0; con<count_nodes[node]; con++)
    //{
    char num[10];
    for (int i = 0; count_connections[node][i] != '\0'; i++)
    {
      /*
      if (count_connections[node][i] != ' ' && count_connections[node][i+1] == ' ')
      {
        int1 = atoi(&count_connections[node][i]);
        (*col_idx)[idx] = int1;
        idx++;
        printf("%d ", int1);
      }
      */
      //printf("%s", &count_connections[node][i]);
      if (count_connections[node][i] != ' ')
      {
        //printf("%s", &count_connections[node][i]);
        int1 = atoi(&count_connections[node][i]);
        sprintf(num, "%d ", int1);
        printf("%s", num);


        //strcat(num, &count_connections[node][i]);
        //printf("%s ", num);
        if (count_connections[node][i+1] == ' ' )
        {
          //printf("New num\n");
          int2 = atoi(num);
          (*col_idx)[idx] = int2;
          idx++;
          char num[10];
        }

      }

    }
    printf("\n");
    //printf("\n");
    //scanf(count_connections[node][con+1]);
    //printf("---%d ", count_connections[node][con+1]);
    //col_idx[node+con] = count_connections[con];
    //printf("%d ", con);
    //}
    //printf("%d ", (*row_ptr)[node]);

  }


  /*
  printf("\nrow pointer:    ");
  for (int i=0; i<*N+1; i++)
  {
    printf("%d ", (*row_ptr)[i]);
  }

  printf("\ncolumn index:   ");
  for (int i=0; i<2*N_edges; i++)
  {
    printf("%d ", (*col_idx)[i]);
  }
  printf("\n");
  */



  free(count_nodes);
  free(count_connections);
  fclose(file);

}










int main()
{
  char *con_file = "connectivity_graph.txt";
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
