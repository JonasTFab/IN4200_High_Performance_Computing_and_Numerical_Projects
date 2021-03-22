# test_programs.c

There two codes only that should be compiled and tested. The first one being `create_connectivity_file.c`. Note that this code is not part of the exam. This is just in addition for creating a connectivity file that can be used in the following programs. More information on how to run this program is given in the next section. The main code in this folder is `test_program.c` and is the only code needed to answer all the tasks. We are considering high-performance computing meaning that parallelization is implemented. Because of this, we need to include the compiler flag `-fopenmp`. So the first step is to compile the code correctly with OpenMP by writing the following in a terminal window.

```bash
$ gcc -fopenmp test_programs.c
```
The program is compiled and a file named `a.out` should be created in the same folder. Before we execute the program, we need to add one or more arguments depending on what we want to run. All arguments must be integers. The first argument represents which program that should be run. You can find the listing below.

Argument  |	Program
---------------------------------------
0         | 	read_graph_from_file1.c
1         | 	read_graph_from_file2.c
2         | 	create_SNN_graph1.c
3         | 	create_SNN_graph2.c
4         | 	check_node.c

Program 0, 1, 2 and 3 requires no additional arguments. Program 4 require two additional arguments. The second argument after applying arguemt 4, denotes which node that is to be checked and the third argument denotes the threshold value tau. So for instance, if you want to run `read_graph_from_file2.c`, you insert the following in your terminal after you have compiled the test program.

```bash
$ ./a.out 1
```
Note that both `read_graph_from_file` programs are runned in `create_SNN_graph` respectively. An example of running `check_node.c` is shown below.

```bash
$ ./a.out 4 1 2
```
This execution example will check if node 1 is in a cluster with a given threshold value of 2. Out of the five mentioned code abode, only the `create_SNN_graph#.c` codes are available for parallelization. When we compile the `test_programs.c` code for the first time, the OpenMP API will automatically detect the maximum number of threads to be used when executing programs. If you want to control the number of threads to be used, you need to define the number of threads before execution understood by OpenMP. This is done after compilation as following.

```bash
$ gcc -fopenmp test_programs.c
$ export OMP_NUM_THREADS=X
```
where X can be any given number. The maximum possible number of threads is limited by the hardware of the computer used.




# create_connectivity_file.c

You can create your own random generated connectivity graph by running this script. The script requires two inputs, that is the total number of nodes and the total number of unique edges. An example of running the script is shown below.

```bash
$ gcc create_connectivity_file.c
$ ./a.out 5 8
```
This command will create a connectivity graph file with five nodes and eight edges. The connectivity is random generated but you can add an additional third argument to give the RNG a seed. Below is a generated file by using a seed number of 0.


```bash
# Undirected graph: connectivity_graph.txt
# Authored by 15806. The connectivity-graph file is generated randomly based on the inserted number of nodes and the number of edges. 
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

This function reads the generated connectivity graph file and outputs the number of nodes and allocates a 2D table of size NxN. The table is then filled with 1 and 0 where 1 means connection between two nodes and 0 means no connection. Column x and row y are presented as node x and node y. Note that since x is connected to y, then y is also connected to x, hence index [x][y] and [y][x] is given the value 1. The results by using the text file generated above is shown below.


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


