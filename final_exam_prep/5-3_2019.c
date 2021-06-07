#include <stdio.h>
#include <string.h>
#include <mpi.h>


int count_occurence(const char *text_string, const char *pattern)
{
  int n_text = strlen(text_string);
  int n_pattern = strlen(pattern);
  int matches = 0;
  //int rank;

  if (my_rank==0)
  {
    printf("n_text: %d     n_pattern: %d \n", n_text, n_pattern);
  }

  for (int i=0; i<n_text-n_pattern+1; i++)
  {
    //rank = omp_get_thread_num();
    //printf("I'm rank %d at index %d \n", rank, i);
    if (strncmp(&text_string[i], pattern, n_pattern) == 0)
    {
      matches++;
    }
  }

  printf("Matches = %d\n", matches);
}



int main()
{
  char test_text[] = "AABABBABAAAAABBABBAABBBABBABA";
  char test_pattern[] = "ABBA";


  MPI_Init(&nargs, &args);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);



  count_occurence(test_text, test_pattern);


  return 0;
}
