# Run double layer convolution

There are three codes in this folder and only one of them should be used for compiling and execution. These are `single_layer_convolution.c`, `MPI_double_layer_convolution.c` and `MPI_main.c`. The latter is the only code needed to compile and execute when testing the programs. Note that `single_layer_convolution.c` is given in the home exam description and is considered as the solution of convolutional computation. The difference between this code and `MPI_double_layer_convolution.c` is the implementation of sequential and parallelization methods. The sequential function is therefore only used to compare the elements from the outputs and time consumption between the programs. Note that `single_layer_convolution.c` is run twice as we are considering a double layer convolution.

Since we are introducing the MPI for parallelization, we need to use a compiler that supports MPI. If you are on a Linux machine, e.g. Ubuntu 20.x, a good solution is to use mpicc for compilation. An example is shown below.

```bash
$ mpicc MPI_main.c
```
Now the program is compiled and has produced the execution file `a.out` which should be in the same folder. Before we can execute `a.out`, we need to understand what happens in `MPI_main.c` first. `MPI_main.c` is coded such that it will set up the 'input' array before it sends it any of the two programs mentioned. Every element in the 'input' array is initiated with random values between 0 and 255. The first and second kernel are created as a horizontal and vertical symmetric quadratic matrices respectively. Therefore, we can not control the element values in each matrix, but what we can control is the length of the dimensions. Hence, four arguments are required when executing the program.

To run the program, we need to include the executable `mpirun -np X` where `X` represents the number of processors the user wants to provide. An example of execution is shown below.

```bash
$ mpirun -np 8 ./a.out 1920 1080 3 3
```
In this example a total of 8 processors is used. The 'input' array has a lengt of 1920 points in one dimension and a length of 1080 points in the other dimension. The two kernels are initiated as two 3 by 3 arrays. When the execution is made, you should get results similar to as shown below.

```
Elapsed timed using 'MPI_double_layer_convolution.c':   0.087755 s
Elapsed timed using 2x 'single_layer_convolution.c':    0.418101 s
Time consumption boost using MPI:                       4.764397
Total non-equal elements:                               0
```
