#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*
This program creates a connectivity graph file
The code should also exclude self-links (FromNodeId==ToNodeId)
and exclude pairwise directed edges ((1,0) and (0,1))
*/


int main(int argc, char* argv[])
{
  if (argc<3)
  {
    printf("Missing argument(s): # nodes and/or # edges!\n");
    printf("Program killed!\n");

    exit(0);
  }

  printf("Creating a new connectivity graph file in .txt format!\n");

  int nodes = atoi(argv[1]);
  int edges = atoi(argv[2]);

  // allocating a 2D array with unique edges
  int **unique_edges = (int**)malloc(edges*sizeof(int*));
  for (int i=0; i<edges; i++)
  {
    unique_edges[i] = (int*)malloc(2*sizeof(int));
  }


  printf("# of nodes:  %d \n", nodes);
  printf("# of edges:  %d \n", edges);


  // writing the connectivity graph text file
  FILE *file;

  file = fopen("connectivity_graph.txt", "w");
  fprintf(file, "# Undirected graph: connectivity_graph.txt\n");
  fprintf(file, "# Authored by 15806, a IN4200 student. The connectivity-\
graph file is generated randomly based on the number of nodes and the \
number of edges. \n");
  fprintf(file, "# Nodes: %d Edges: %d\n", nodes, edges);
  fprintf(file, "# FromNodeId ToNodeId\n");

  int int1, int2, statement;


  //srandom(time(NULL));      // random seed
  srandom(0);           // fixed seed
  for (int edge=0; edge<edges; edge++)
  {
    int1 = nodes*random()/RAND_MAX;
    int2 = nodes*random()/RAND_MAX;


    statement = 1;
    while (statement==1)
    {
      statement = 0;

      // rejecting self-links
      while (int1 == int2)
      {
        int2 = nodes*random()/RAND_MAX;
        statement = 1;
      }

      // making sure of unique listings
      if (statement==0)
      {
        for (int i=0; i<edge; i++)
        {
          if (int1==unique_edges[i][0] && int2==unique_edges[i][1])
          {
            int1 = nodes*random()/RAND_MAX;
            int2 = nodes*random()/RAND_MAX;
            statement = 1;
          }

          else if (int1==unique_edges[i][1] && int2==unique_edges[i][0])
          {
            int1 = nodes*random()/RAND_MAX;
            int2 = nodes*random()/RAND_MAX;
            statement = 1;
          }
        }
      }

      unique_edges[edge][0] = int1;
      unique_edges[edge][1] = int2;
    }

    fprintf(file, "%d  %d\n", int1, int2);
  }

  fclose(file);

  return 0;
}
