#include <stdio.h>


void task_a()
{
  int i, sign;
  float out, denum;
  out = 0;

  for (i=0; i<10; i++)
  {
    // setting correct sign
    if (i&2 == 0)
    {
      sign = 1;
    }

    else
    {
      sign = -1;
    }


    out = out + sign * 1 / (2);
    printf("%f \n", out);
  }
}


int main()
{
  //  First exercise in IN4200
  // a)
  task_a();



  return 0;
}
