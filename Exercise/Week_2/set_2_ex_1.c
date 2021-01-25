#include <stdio.h>
#include <stdlib.h>
#include <time.h>



void task_a()
{

  // timing of reading and writing ascii files
  FILE *ascii_in;
  FILE *ascii_out;
  char line[100];
  clock_t t0 = clock();
  clock_t tot_time;

  ascii_in = fopen("txt_files/star_wars.txt", "r");
  ascii_out = fopen("txt_files/new_star_wars_ascii.txt", "w");

  // checks if the files do exists or not
  if (ascii_in == NULL || ascii_out == NULL)
  {
    printf("\nAscii files not found!\n\n");
    exit(-1);
  }

  while (fgets(line, 100, ascii_in) != 0)
  {
    //printf("%s", line);     // prints the content of file
    fputs(line, ascii_out);
  }

  fclose(ascii_in);
  fclose(ascii_out);

  tot_time = clock() - t0;
  printf("Time of ascii reading and writing: %f s\n", ((float)tot_time) / CLOCKS_PER_SEC);





  // timing of reading and writing binary files
  FILE *binary_in;
  FILE *binary_out;
  t0 = clock();

  binary_in = fopen("txt_files/star_wars.txt", "rb");
  binary_out = fopen("txt_files/new_star_wars_binary.txt", "wb");

  // checks if the files exists or not
  if (binary_in == NULL || binary_out == NULL)
  {
    printf("\nBinary files not found!\n\n");
    exit(-1);
  }

  while (fgets(line, 100, binary_in) != 0)
  {
    //printf("%s", line);     // prints the content of file
    fputs(line, binary_out);
  }

  fclose(binary_in);
  fclose(binary_out);

  clock_t tot_time_b = clock() - t0;
  printf("Time of binary reading and writing: %f s\n\n", ((float)tot_time_b) / CLOCKS_PER_SEC);

  printf("The ratio of ascii/binary reading and writing is appoximately: %f\n",\
          ((float)tot_time/(float)tot_time_b));



}








int main()
{

  task_a();






  return 0;
}
