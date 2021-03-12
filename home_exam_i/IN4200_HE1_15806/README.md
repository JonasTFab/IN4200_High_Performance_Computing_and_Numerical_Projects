# create_connectivity_file.c

You can create your own random generated connectivity graph by running this script. The script requires two inputs, that is the total number of nodes and the total number of unique edges. An example of running the script is shown below.

```bash
$ gcc create_connectivity_file.c
$ ./a.out 5 8
```
This command will create a connectivity graph file with five nodes and eight edges. The connectivity is random generated but you can add an additional third argument to give the RNG a seed. Below is a generated file by using a seed of 0.


```bash
# Undirected graph: connectivity_graph.txt
# Authored by a IN4200 student. The connectivity-graph file is generated randomly based on the number of nodes and the number of edges. 
# Nodes: 5 Edges: 8
# FromNodeId ToNodeId
4  1
3  4
0  1
3  1
2  3
1  2
3  0
4  0
```




# read_graph_from_file1.c

This function reads the generated connectivity graph file and outputs the number of nodes as and allocates a 2D table of size NxN. The table is then filled with 1 and 0 where 1 means connection between two nodes and 0 means no connection. Row x and column y are presented as node x and node y. Note that since x is connected to y, then y is also connected to x, hence index [x][y] and [y][x] is given the value 1. The results by using the script above is shown below.


```bash
$ gcc read_graph_from_file1.c
$ ./a.out

Nodes:   5
Printing 2D table: 
0  1  0  1  1  
1  0  1  1  1  
0  1  0  1  0  
1  1  1  0  1  
1  1  0  1  0
```






# read_graph_from_file2.c

Some text



# create_SNN_graph1.c

Some text



# create_SNN_graph2.c

Some text



# test_programs.c

Some text


