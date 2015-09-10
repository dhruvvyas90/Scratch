#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
  struct timeval tv;
  struct timeval start_tv;
  char *command = "gpio write 1 1"; 
  gettimeofday(&start_tv, NULL);

  system(command);

  double elapsed = 0.0;

  gettimeofday(&tv, NULL);
  elapsed = (tv.tv_sec - start_tv.tv_sec) +
    (tv.tv_usec - start_tv.tv_usec) / 1000000.0;
  printf("Time elapsed is : %lf\n",elapsed);
  return 0;
}
