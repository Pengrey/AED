////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AED, 2020/2021
//
// TODO: place the student number and name here
// TODO: place the student number and name here (if applicable)
// TODO: place the student number and name here (if applicable)
//
// Brute-force solution of the generalized weighted job selection problem
//
// Compile with "cc -Wall -O2 job_selection.c -lm" or equivalent
//
// In the generalized weighted job selection problem we will solve here we have T programming tasks and P programmers.
// Each programming task has a starting date (an integer), an ending date (another integer), and a profit (yet another
// integer). Each programming task can be either left undone or it can be done by a single programmer. At any given
// date each programmer can be either idle or it can be working on a programming task. The goal is to select the
// programming tasks that generate the largest profit.
//
// Things to do:
//   0. (mandatory)
//      Place the student numbers and names at the top of this file.
//   1. (highly recommended)
//      Read and understand this code.
//   2. (mandatory)
//      Solve the problem for each student number of the group and for
//        N=1, 2, ..., as higher as you can get and
//        P=1, 2, ... min(8,N)
//      Present the best profits in a table (one table per student number).
//      Present all execution times in a graph (use a different color for the times of each student number).
//      Draw the solutions for the highest N you were able to do.
//   3. (optional)
//      Ignore the profits (or, what is the same, make all profits equal); what is the largest number of programming
//      tasks that can be done?
//   4. (optional)
//      Count the number of valid task assignments. Calculate and display an histogram of the number of occurrences of
//      each total profit. Does it follow approximately a normal distribution?
//   5. (optional)
//      Try to improve the execution time of the program (use the branch-and-bound technique).
//      Can you use divide and conquer to solve this problem?
//      Can you use dynamic programming to solve this problem?
//   6. (optional)
//      For each problem size, and each student number of the group, generate one million (or more!) valid random
//      assignments and compute the best solution found in this way. Compare these solutions with the ones found in
//      item 2.
//   7. (optional)
//      Surprise us, by doing something more!
//   8. (mandatory)
//      Write a report explaining what you did. Do not forget to put all your code in an appendix.

//   9. Resolver problema de ob_selection sem ser generalizada
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../P02/elapsed_time.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Random number generator interface (do not change anything in this code section)
//
// In order to ensure reproducible results on Windows and GNU/Linux, we use a good random number generator, available at
//   https://www-cs-faculty.stanford.edu/~knuth/programs/rng.c
// This file has to be used without any modifications, so we take care of the main function that is there by applying
// some C preprocessor tricks
//

#define main  rng_main                        // main gets replaced by rng_main
#ifdef __GNUC__
int rng_main() __attribute__((__unused__));   // gcc will not complain if rnd_main() is not used
#endif
#include "rng.c"
#undef main                                   // main becomes main again
#define srandom(seed)  ran_start((long)seed)  // start the pseudo-random number generator
#define random()       ran_arr_next()         // get the next pseudo-random number (0 to 2^30-1)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// problem data (if necessary, add new data fields in the structures; do not change anything else in this code section)
//
// on the data structures declared below, a comment starting with
// * a I means that the corresponding field is initialized by init_problem()
// * a S means that the corresponding field should be used when trying all possible cases
// * IS means both (part initialized, part used)
//

#if 1

#define MAX_T  64  // maximum number of programming tasks
#define MAX_P  10  // maximum number of programmers

typedef struct
{
  int starting_date;      // I starting date of this task
  int ending_date;        // I ending date of this task
  int profit;             // I the profit if this task is performed
  int assigned_to;        // S current programmer number this task is assigned to (use -1 for no assignment)
  int best_assigned_to;        // S current programmer number this task is assigned to (use -1 for no assignment)
}
task_t;

typedef struct
{
  int path_length;      // I numero de elementos no array
  int pathArray[MAX_T]; // S? the size of this array will is the problem T_T
} path_t;
typedef struct
{
  int NMec;               // I  student number
  int T;                  // I  number of tasks
  int P;                  // I  number of programmers
  int I;                  // I  if 1, ignore profits
  int total_profit;       // S  current total profit
  int biggestP;       // S  current total profit
  double cpu_time;        // S  time it took to find the solution
  task_t task[MAX_T];     // IS task data
  int busy[MAX_P];        // S  for each programmer, record until when she/he is busy (-1 means idle)
  char dir_name[16];      // I  directory name where the solution file will be created
  char file_name[64];     // I  file name where the solution data will be stored
  int casos;
  path_t path[MAX_T]; //NÃO GOSTO DE USAR MAX_T AQUI, SÓ SERIA NECESSÁRIO USAR T!!
}
problem_t;
//////////////////////////////////////////////////////////////
//
//  Recurse<->
//
int recurse(problem_t *prob,int t)
{

  int busy_save;
  int profit_save;
  int task_save;
  if(t >= prob->T)
  {
    prob->casos++;
    if(prob->total_profit > prob->biggestP)
    {
      prob->biggestP = prob->total_profit;
      for(int j = 0 ; j < prob->T; j++)
      {
        prob->task[j].best_assigned_to = prob->task[j].assigned_to;
      }
    }
    return 1;
  }
  recurse(prob, t + 1);
  for(int p = 0; p < prob->P; p++)
  {
    if(prob->busy[p] < prob->task[t].starting_date)
    {
      if(prob->task[t].assigned_to < 0)
      {
        busy_save = prob->busy[p];
        task_save = prob->task[t].assigned_to;
        profit_save = prob->total_profit;

        prob->busy[p]= prob->task[t].ending_date;
        prob->task[t].assigned_to = p ;
        prob->total_profit += prob->task[t].profit;
        recurse(prob, t+1);
        prob->busy[p]= busy_save ;
        prob->task[t].assigned_to = task_save ;
        prob->total_profit = profit_save;
        return 1;
      }
    }
  }
  return 0;
}
////////////////////////////////////////////////////////////
//
//Recurse biggest length para 1 prog
//
int Recurse(problem_t *prob,int t)
{
  int busy_save;
  int profit_save;
  int task_save;
  if(t >= prob->T)
  {
    prob->casos++;
    if(prob->total_profit > prob->biggestP)
    {
      prob->biggestP = prob->total_profit;
      for(int j = 0 ; j < prob->T; j++)
      {
        prob->task[j].best_assigned_to = prob->task[j].assigned_to;
      }
    }
    return 1;
  }
  recurse(prob, t + 1);
  if(prob->busy[0] > prob->task[t].ending_date)
  {
    if(prob->task[t].assigned_to < 0)
    {
      busy_save = prob->busy[0];
      task_save = prob->task[t].assigned_to;
      profit_save = prob->total_profit;

      prob->busy[0]= prob->task[t].ending_date;
      prob->task[t].assigned_to = 0 ;
      prob->total_profit += prob->task[t].profit;
      recurse(prob, t+1);
      prob->busy[0]= busy_save ;
      prob->task[t].assigned_to = task_save ;
      prob->total_profit = profit_save;
      return 1;
    }
  }
  return 0;
}
////////////////////////////////////////////////////////////
//
//nonRecurse biggest length para 1 prog
//
int nonRecurse(problem_t *prob)
{
  prob->busy[0] = prob->task[0].ending_date + 1;
  for(int t = 0; t < prob->T; t++)
  {
    if(prob->busy[0] > prob->task[t].ending_date)
    {
      if(prob->task[t].assigned_to < 0)
      {
        prob->busy[0]= prob->task[t].starting_date;
        prob->task[t].assigned_to = 0 ;
        prob->total_profit += prob->task[t].profit;
      }
    }
  }
  if(prob->total_profit > prob->biggestP)
  {
    prob->biggestP = prob->total_profit;
    for(int j = 0 ; j < prob->T; j++)
    {
      prob->task[j].best_assigned_to = prob->task[j].assigned_to;
    }
  }
  return 0;
}



int compare_tasks_E(const void *t1,const void *t2)
{
  int d1,d2;

  d1 = ((task_t *)t1)->ending_date;
  d2 = ((task_t *)t2)->ending_date;
  if(d1 != d2)
    return (d1 > d2) ? -1 : +1;
  d1 = ((task_t *)t1)->starting_date;
  d2 = ((task_t *)t2)->starting_date;
  if(d1 != d2)
    return (d1 > d2) ? -1 : +1;
  return 0;
}

int compare_tasks(const void *t1,const void *t2)
{
  int d1,d2;

  d1 = ((task_t *)t1)->starting_date;
  d2 = ((task_t *)t2)->starting_date;
  if(d1 != d2)
    return (d1 < d2) ? -1 : +1;
  d1 = ((task_t *)t1)->ending_date;
  d2 = ((task_t *)t2)->ending_date;
  if(d1 != d2)
    return (d1 < d2) ? -1 : +1;
  return 0;
}

void init_problem(int NMec,int T,int P,int ignore_profit,problem_t *problem)
{
  int i,r,scale,span,total_span;
  int *weight;

  //
  // input validation
  //
  if(NMec < 1 || NMec > 999999)
  {
    fprintf(stderr,"Bad NMec (1 <= NMex (%d) <= 999999)\n",NMec);
    exit(1);
  }
  if(T < 1 || T > MAX_T)
  {
    fprintf(stderr,"Bad T (1 <= T (%d) <= %d)\n",T,MAX_T);
    exit(1);
  }
  if(P < 1 || P > MAX_P)
  {
    fprintf(stderr,"Bad P (1 <= P (%d) <= %d)\n",P,MAX_P);
    exit(1);
  }
  //
  // the starting and ending dates of each task satisfy 0 <= starting_date <= ending_date <= total_span
  //
  total_span = (10 * T + P - 1) / P;
  if(total_span < 30)
    total_span = 30;
  //
  // probability of each possible task duration
  //
  // task span relative probabilities
  //
  // |  0  0  4  6  8 10 12 14 16 18 | 20 | 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1 | smaller than 1
  // |  0  0  2  3  4  5  6  7  8  9 | 10 | 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 | 30 31 ... span
  //
  weight = (int *)alloca((size_t)(total_span + 1) * sizeof(int)); // allocate memory (freed automatically)
  if(weight == NULL)
  {
    fprintf(stderr,"Strange! Unable to allocate memory\n");
    exit(1);
  }
#define sum1  (298.0)                      // sum of weight[i] for i=2,...,29 using the data given in the comment above
#define sum2  ((double)(total_span - 29))  // sum of weight[i] for i=30,...,data_span using a weight of 1
#define tail  100
  scale = (int)ceil((double)tail * 10.0 * sum2 / sum1); // we want that scale*sum1 >= 10*tail*sum2, so that large task
  if(scale < tail)                                      // durations occur 10% of the time
    scale = tail;
  weight[0] = 0;
  weight[1] = 0;
  for(i = 2;i <= 10;i++)
    weight[i] = scale * (2 * i);
  for(i = 11;i <= 29;i++)
    weight[i] = scale * (30 - i);
  for(i = 30;i <= total_span;i++)
    weight[i] = tail;
#undef sum1
#undef sum2
#undef tail
  //
  // accumulate the weigths (cummulative distribution)
  //
  for(i = 1;i <= total_span;i++)
    weight[i] += weight[i - 1];
  //
  // generate the random tasks
  //
  srandom(NMec + 314161 * T + 271829 * P);
  problem->NMec = NMec;
  problem->T = T;
  problem->P = P;
  problem->I = (ignore_profit == 0) ? 0 : 1;
  for(i = 0;i < T;i++)
  {
    //
    // task starting an ending dates
    //
    r = 1 + (int)random() % weight[total_span]; // 1 .. weight[total_span]
    for(span = 0;span < total_span;span++)
      if(r <= weight[span])
        break;
    problem->task[i].starting_date = (int)random() % (total_span - span + 1);
    problem->task[i].ending_date = problem->task[i].starting_date + span - 1;
    //
    // task profit
    //
    // the task profit is given by r*task_span, where r is a random variable in the range 50..300 with a probability
    //   density function with shape (two triangles, the area of the second is 4 times the area of the first)
    //
    //      *
    //     /|   *
    //    / |       *
    //   /  |           *
    //  *---*---------------*
    // 50 100 150 200 250 300
    //
    scale = (int)random() % 12501; // almost uniformly distributed in 0..12500
    if(scale <= 2500)
      problem->task[i].profit = 1 + round((double)span * (50.0 + sqrt((double)scale)));
    else
      problem->task[i].profit = 1 + round((double)span * (300.0 - 2.0 * sqrt((double)(12500 - scale))));
  }
  //
  // sort the tasks by the starting date
  //
  qsort((void *)&problem->task[0],(size_t)problem->T,sizeof(problem->task[0]),compare_tasks);
  //
  // finish
  //
  if(problem->I != 0)
    for(i = 0;i < problem->T;i++)
      problem->task[i].profit = 1;
#define DIR_NAME  problem->dir_name
  if(snprintf(DIR_NAME,sizeof(DIR_NAME),"%06d",NMec) >= sizeof(DIR_NAME))
  {
    fprintf(stderr,"Directory name too large!\n");
    exit(1);
  }
#undef DIR_NAME
#define FILE_NAME  problem->file_name
  if(snprintf(FILE_NAME,sizeof(FILE_NAME),"%06d/%02d_%02d_%d.txt",NMec,T,P,problem->I) >= sizeof(FILE_NAME))
  {
    fprintf(stderr,"File name too large!\n");
    exit(1);
  }
#undef FILE_NAME
}

#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// problem solution (place your solution here)======================================================================



int actualRecurseOne(int j, int i, /*int arr[][3]*/ task_t *task, int num_tasks, path_t *path)
{ //i - task a ser trabalhada agora (nº da task) / j- ultima task que foi aceite

  static int prev;
  prev = task[j].ending_date; // prev = valor do tempo final da task previamente
  if (i > num_tasks)
  { // caso terminal, ao fim do array task acabar recur  NOTAS: 0--starting_date  | 1--ending_date
    path->path_length++;
    path->pathArray[i]=j;
    return 1;
  }
  if (prev < task[i].starting_date)       //  arr[i][0]
  {                                       // caso a task a ser avaliada tenha uma data de começo superior à data final da ultima task do nosso path
    int besti = i;                        // define best case do i para que seja um tempo de finalização curto
    int next = task[i + 1].starting_date; // define next como o próximo no array das tasks para verificar se possuem a mesma starting date
    while (task[i].starting_date == next)
    {                                                    // while de verificação se o próximo possui mesma starting date
      i++;                                               // incremento do i
      next = task[i + 1].starting_date;                  // defenir next como o próximo elemento a avaliar
      if (task[besti].ending_date > task[i].ending_date) // [besti][2] transformedinto task[besti].ending_date
      {                                                  // caso o i tenha um valor final inferior escolher esse como preferência
        besti = i;                                       // definir o i preferivel
      }                                                  //
    }                                                    //
    j = besti;                                           //
    printf("Atual i = %d\n", i);                         // print da task atualmente a ser avaliada (debugg)
    prev = task[j].ending_date;                          // prev fica com o valor da data final da task a ser avaliada


    path->pathArray[i] = j;
    printf("DEBUG: j= %d ; pathArray[i]= %d\n previous one %d\n",j,path->pathArray[i],path->pathArray[i+1]);

    path->path_length++;
    printf("BIG DEBUG: ADDED ONE TO PATH_LENGHT. PATH LENGTH NOW %d\n",path->path_length);


    i++;                                                      // adicionar +1 a i para testar a task seguinte

    return 1 + actualRecurseOne(j, i, task, num_tasks, path); // soma de +1 ao result caso se adicione uma task ao path
  }
  else
  {                                                           //
    i++;                                                      // pedido da taks seguinte para teste
    return 0 + actualRecurseOne(j, i, task, num_tasks, path); // soma de +0 ao result caso não se adicione uma task ao path
  }                                                           //
} //
//
int recurseOne(problem_t *problem)
{ // algoritmo para teste de paths
  for (int p = 0; p < problem->T; p++)
  {                                 // iteração pelas tasks iniciais (nº de tasks)
    printf("Working for: %d\n", p); // print da task k atualmente se começou (debugg)
    printf("Atual i = %d\n", p);    // print da task inicial (debugg)
    path_t* fuckpointers = &(problem->path[p]);
    int result = actualRecurseOne(p, 0, problem->task, problem->T, fuckpointers); // result = iniciação de recur começado em p
    printf("BIG BIG DEBUG: %d\n",fuckpointers->path_length);
    printf("Number of tasks: %d\n\n", result); // print do numero de tasks num path (debugg)
  }
  return 0;
}
//

#if 1

static void solve(problem_t *problem)
{
  FILE *fp;
  int i;

  //
  // open log file
  //
  (void)mkdir(problem->dir_name, S_IRUSR | S_IWUSR | S_IXUSR);
  fp = fopen(problem->file_name, "w"); // add
  if(fp == NULL)
  {
    fprintf(stderr,"Unable to create file %s (maybe it already exists? If so, delete it!)\n",problem->file_name);
    exit(1);
  }
  //
  // solve
  //
  problem->cpu_time = cpu_time();

  for(int p = 0; p < problem->P;p++)
  {
    problem->busy[p] = -1;
  }
  for(int t = 0; t < problem->T; t++)
  {
    problem->task[t].assigned_to = -1;
    problem->task[t].best_assigned_to = -1;
  }
  problem->total_profit = 0;
  problem->biggestP = 0;
  problem->casos = 0;
    // call your (recursive?) function to solve the problem here
  if(problem->I == 1 && problem->P == 1)
  {
    qsort((void *)&problem->task[0],(size_t)problem->T,sizeof(problem->task[0]),compare_tasks_E);
    //nonRecurse(problem);
    Recurse(problem, 0);
  }
  recurse(problem,0);


  for(int t = 0; t < problem->T; t++)
  {
    printf("Best Assigned to da task %d, e no prog %d\n",t,problem->task[t].best_assigned_to);
  }
  printf("Biggest P = %d\n",problem->biggestP);
  printf("Casos  viaveis= %d\n",problem->casos);
  problem->cpu_time = cpu_time() - problem->cpu_time;
  printf("Solution time = %.3e\n",problem->cpu_time);
  //
  // save solution data
  //

  fprintf(fp,"R NMec = %d\n",problem->NMec);
  fprintf(fp,"T = %d\n",problem->T);
  fprintf(fp,"P = %d\n",problem->P);
  fprintf(fp,"Profits%s ignored\n",(problem->I == 0) ? " not" : "");
  fprintf(fp,"Solution time = %.3e\n",problem->cpu_time);
  fprintf(fp,"Task data\n");
#define TASK  problem->task[i]
  for(i = 0;i < problem->T;i++)
    fprintf(fp,"  %3d %3d %5d\n",TASK.starting_date,TASK.ending_date,TASK.profit);
#undef TASK
  fprintf(fp,"End\n");
  //
  // terminate
  //
  if(fflush(fp) != 0 || ferror(fp) != 0 || fclose(fp) != 0)
  {
    fprintf(stderr,"Error while writing data to file %s\n",problem->file_name);
    exit(1);
  }
}

#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// main program
//

int main(int argc,char **argv)
{
  problem_t problem;
  int NMec,T,P,I;

  NMec = (argc < 2) ? 2020 : atoi(argv[1]);
  T = (argc < 3) ? 5 : atoi(argv[2]);
  P = (argc < 4) ? 2 : atoi(argv[3]);
  I = (argc < 5) ? 0 : atoi(argv[4]);
  init_problem(NMec,T,P,I,&problem);
  solve(&problem);
  return 0;
}
