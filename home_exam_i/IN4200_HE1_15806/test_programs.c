#include "read_graph_from_file1.c"
#include "read_graph_from_file2.c"
#include "create_SNN_graph1.c"
//#include "create_SNN_graph2.c"



int main()
{
   // file to be used  during test runs
  //char *con_file = "connectivity_graph.txt";
  char *con_file = "simple-graph.txt";


  // ------------ testing read_graph_from_file1.c ------------
  // read a text file and from it, reads number of nodes
  // and allocate the connectivty graph as a 2D table
  /*
  int nodes;
  char **table;
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
  // deallocate table from memory
  free(table);
  */



  // ------------ testing read_graph_from_file2.c ------------

  int nodes;
  int *row_pointer, *column_index;
  // read a text file and from it, ...
  //
  read_graph_from_file2(con_file, &nodes, &row_pointer, &column_index);
  // deallocate arrays from memory
  free(row_pointer);
  free(column_index);




  // ------------ testing create_SNN_graph1.c ------------
  /*
  int nodes;
  char **table;
  int **table_SNN;

  read_graph_from_file1(con_file, &nodes, &table);
  create_SNN_graph1(nodes, table, &table_SNN);

  // deallocate arrays from memory
  free(table);
  free(table_SNN);
  */



  // ------------ testing create_SNN_graph2.c ------------





}
