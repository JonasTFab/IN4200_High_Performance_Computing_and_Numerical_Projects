#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "read_graph_from_file1.c"
#include "read_graph_from_file2.c"
#include "create_SNN_graph1.c"
#include "create_SNN_graph2.c"
#include "check_node.c"


/*
This is the only program that needs to run.
The input index should represent which of the
five additional program that will be tested.
0: read_graph_from_file1.c
1: read_graph_from_file2.c
2: create_SNN_graph1.c
3: create_SNN_graph2.c
4: check_node.c
Note that both 2 and 3 are coded such that it may be
runned in a parallized behavior. Therefore the number
of threads used should be specified when compiling.
*/


int main(int argc, char* argv[])
{

  // an argument is required to run this program
  if (argc<2)
  {
    printf("Missing argument!\n");
    printf("Please insert argument between 0 and 4.\n");
    printf("0: read_graph_from_file1.c\n");
    printf("1: read_graph_from_file2.c\n");
    printf("2: create_SNN_graph1.c\n");
    printf("3: create_SNN_graph2.c\n");
    printf("4: check_node.c (Requires two additional arguments; ");
    printf("the node to check and a threshold value (for clustering)).\n");
    printf("Program killed!\n");
    exit(0);
  }

  int arg = atof(argv[1]);
  int node, threshold;
  if (arg==4)
  {
    if (argc<4)
    {
      printf("Missing arguments!\n");
      printf("Please insert a node to check and a threshold \n");
      printf("value to define a possible cluster.\n");
      printf("Program killed!\n");
      exit(0);
    }
    else
    {
      node = atof(argv[2]);
      threshold = atof(argv[3]);
    }
  }


  // file to read during test runs
  //char *con_file = "simple-graph.txt";
  //char *con_file = "connectivity_graph.txt";
  char *con_file = "facebook_combined.txt";


  // ------------ testing read_graph_from_file1.c ------------
  if (arg==0)
  {
    // read a text file and from it, reads number of nodes
    // and allocate the connectivty graph as a 2D table

    int nodes;
    char **table;

    read_graph_from_file1(con_file, &nodes, &table);

    // deallocate array from memory
    free(table);

  }


  // ------------ testing read_graph_from_file2.c ------------
  else if (arg==1)
  {
    // read a text file and from it, creates two arrays
    // that is filled with values in a compressed
    // row storage format (CRS)

    int nodes;
    int *row_pointer, *column_index;

    read_graph_from_file2(con_file, &nodes, &row_pointer, &column_index);

    // deallocate arrays from memory
    free(row_pointer);
    free(column_index);

  }


  // ------------ testing create_SNN_graph1.c ------------
  else if (arg==2)
  {
    // using the 2D table read from file by
    // read_graph_from_file1.c and creates another 2D
    // shared nearest neighbour (SNN) table

    int nodes, threads;
    char **table;
    int **table_SNN;
    double t0,t1;

    #pragma omp parallel
    {
      threads = omp_get_num_threads();
    }

    printf("Read and create a SNN table from text file using %d thread(s): %s\
            \n", threads, con_file);
    t0 = omp_get_wtime();
    read_graph_from_file1(con_file, &nodes, &table);
    create_SNN_graph1(nodes, table, &table_SNN);
    t1 = omp_get_wtime();
    printf("Time used:     %.3f s\n", t1-t0);

    // deallocate arrays from memory
    free(table);
    free(table_SNN);

  }



  // ------------ testing create_SNN_graph2.c ------------
  else if (arg==3)
  {
    // using the two CRS arrays read from file by
    // read_graph_from_file2.c and creates an
    // additional SNN array

    int nodes, threads;
    int *row_pointer, *column_index, *SNN_values;
    double t0,t1;

    #pragma omp parallel
    {
      threads = omp_get_num_threads();
    }

    printf("Read and create a SNN list from text file using %d thread(s): %s\
            \n", threads, con_file);
    t0 = omp_get_wtime();
    read_graph_from_file2(con_file, &nodes, &row_pointer, &column_index);
    create_SNN_graph2(nodes, row_pointer, column_index, &SNN_values);
    t1 = omp_get_wtime();
    printf("Time used:     %.3f s\n", t1-t0);

    // deallocate arrays from memory
    free(row_pointer);
    free(column_index);
    free(SNN_values);

  }


  // ------------ testing check_node.c ------------
  else if (arg==4)
  {
    // checks if a node is  placed in a cluster
    // with a given threshold

    int nodes;
    int *row_pointer, *column_index, *SNN_values;

    read_graph_from_file2(con_file, &nodes, &row_pointer, &column_index);
    create_SNN_graph2(nodes, row_pointer, column_index, &SNN_values);
    check_node(node, threshold, nodes, row_pointer, column_index, SNN_values);

    // deallocate arrays from memory
    free(row_pointer);
    free(column_index);
    free(SNN_values);

  }

  // ------------ prints that argument is not valid ------------
  else
  {
    printf("Argument (%d) is not valid!\n", arg);
    printf("Please insert argument between 0 and 4.\n");
    printf("0: read_graph_from_file1.c\n");
    printf("1: read_graph_from_file2.c\n");
    printf("2: create_SNN_graph1.c\n");
    printf("3: create_SNN_graph2.c\n");
    printf("4: check_node.c (Requires two additional arguments; ");
    printf("the node to check and a threshold value (for clustering)).\n");
  }


} // end of main
