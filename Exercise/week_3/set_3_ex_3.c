#include <stdio.h>
#include <math.h>
#include <time.h>


double new_pow(double x, int y)
{
  double x_return = x;

  for (int i=1; i<y; i++)
  {
    x_return *= x;
  }
}





int main()
{

  double x = 1.01;
  int power = 100;


  time_t t0 = clock();
  double pow_c = pow(x, power);
  time_t time_c = clock() - t0;
  printf("Computed value from c library:    %f\n", pow_c);

  t0 = clock();
  double pow_own = new_pow(x, power);
  time_t time_own = clock() - t0;
  printf("Computed value from own script:   %f\n", pow_own);


  printf("Relative time usage (c library/own code): %f\n", ((double)time_c/(double)time_own));



  return 0;
}
