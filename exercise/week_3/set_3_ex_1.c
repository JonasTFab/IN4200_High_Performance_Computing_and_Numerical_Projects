#include <stdio.h>
#include <time.h>


double numerical_integration (double x_min, double x_max, int slices)
{
  double delta_x = (x_max-x_min)/slices;
  double x, sum = 0.0;

  for (int i=0; i<slices; i++)
  {
    x = x_min + (i+0.5)*delta_x;
    sum = sum + 4.0/(1.0+x*x);
  }

  return sum*delta_x;
}




int main()
{

  // testing the integration function by integrate from 0 to 1
  // which should covnert to pi with increase of integration steps
  double integrate;
  double x_min = 0.;
  double x_max = 1.;
  int steps = 10;

  for (int i=1; i<steps; i++)
  {
    integrate = numerical_integration(x_min, x_max, i);
    printf("Integration steps: %d    Integration: %f\n", i, integrate);
  }
  printf("We note that the integration function converts to pi with ");
  printf("respect to total number of integration steps!\n");


  // calculating latency of floating-point division in clock cycles
  int num_int_steps = 10000;
  time_t t0, t1;

  t0 = clock();
  numerical_integration(0, 1, num_int_steps);
  t1 = clock();

  printf("Latency of integration: %f\n", (((double)t1-(double)t0)/(double)num_int_steps));






  return 0;
}
