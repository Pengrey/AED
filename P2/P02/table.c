//
// Tomás Oliveira e Silva, AED, September 2020
//
// program to print a table of the squares and square roots of some integers
//
// on GNU/Linux, run the command
//   man 3 printf
// to see the manual page of the printf function
//

#include <math.h>
#include <stdio.h>

void do_it(int N)
{
  int i;

  FILE *fp;
  fp = fopen("table.txt", "w");
  fprintf(fp, " n    sin    cos\n");
  fprintf(fp, "-- ------ ------\n");
  for(i = 1;i <= N;i++)
    fprintf(fp, "%2d %3.4f %3.4f\n",i,sin(i * (M_PI/180)),cos(i * (M_PI/180)));
  fclose(fp);
}

int main(void)
{
  do_it(90);
  printf("DONE!\n");
  return 0;
}
