#include <stdio.h>
#include <string.h>
#include <time.h>


int count_occurence(const char *text_string, const char *pattern)
{
  int n_text = strlen(text_string);
  int n_pattern = strlen(pattern);
  int matches = 0;
  printf("n_text: %d     n_pattern: %d \n", n_text, n_pattern);

  for (int i=0; i<n_text-n_pattern+1; i++)
  {
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

  time_t t_i = clock();
  count_occurence(test_text, test_pattern);
  time_t t_f = clock();
  printf("Total clock cycles: %ld \n", t_f-t_i);

  return 0;
}
