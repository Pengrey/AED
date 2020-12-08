package java_solution;

import java.util.Arrays;
import java.util.Random;

public class Main {
	static int tail = 100;
	static int sum1 = 298;
	static int MAX_T = 64;
	static int MAX_P = 10;

	public static void main(String argv[]) {
	  Problem_T problem = new Problem_T();
	  Integer NMec,T,P,I;
	  int argc = argv.length;
		NMec = (argc < 2) ? 2020 : Integer.parseInt(argv[1]) ;
	  T = (argc < 3) ? 5 : Integer.parseInt(argv[2]);
	  P = (argc < 4) ? 2 : Integer.parseInt(argv[3]);
	  I = (argc < 5) ? 0 : Integer.parseInt(argv[4]);
	  init_problem(NMec,T,P,I,problem);
	  solve(problem);
	 
	}
	public static void init_problem(Integer nmec, Integer T, Integer P, Integer I, Problem_T problem) {
		  int i,r,scale,span,total_span;
		  //
		  // input validation
		  //

		  if(nmec < 1 || nmec > 999999)
		  {
		    System.err.print(String.format("Bad NMec (1 <= NMex (%d) <= 999999)\n",nmec));
		    System.exit(-1);
		  }
		  if(T < 1 || T > MAX_T)
		  {
		    System.err.print(String.format("Bad T (1 <= T (%d) <= %d)\n",T,MAX_T));
			    System.exit(-1);
		  }
		  if(P < 1 || P > MAX_P)
		  {
		  	System.err.print(String.format("Bad P (1 <= P (%d) <= %d)\n",P,MAX_P));
		    System.exit(-1);
		  }
		  total_span = (10 * T + P - 1) / P;
		  if(total_span < 30)
		    total_span = 30;
		  final double sum2 = total_span - 1;
		  int[] weight = new int[total_span + 1];
		  scale = (int) Math.ceil((double) tail * 10.0 * sum2 / sum1);
		  if(scale < tail) scale = tail;
		  weight[0] = 0;
		  weight[1] = 0;
		  for(i = 2;i <= 10;i++)
		    weight[i] = scale * (2 * i);
		  for(i = 11;i <= 29;i++)
		    weight[i] = scale * (30 - i);
		  for(i = 30;i <= total_span;i++)
		    weight[i] = tail;
		  for(i = 1;i <= total_span;i++)
		    weight[i] += weight[i - 1];
		  Random random = new Random();
		  random.setSeed(nmec + 314161 * T + 271829 * P);
		  problem.setNMec(nmec);
		  problem.setP(P);
		  problem.setT(T);
		  problem.setI(I);
		  for(i = 0;i < T;i++)
		  {
		    r = 1 + random.nextInt() % weight[total_span]; // 1 .. weight[total_span]
		    for(span = 0;span < total_span;span++)
		      if(r <= weight[span])
		        break;
		    problem.task[i].setStartingDate((int)random.nextInt() % (total_span - span + 1 ));
		    problem.task[i].setEndingDate(problem.task[i].getStarting_date() + span - 1);
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
		    scale = (int)random.nextInt() % 12501; // almost uniformly distributed in 0..12500
		    if(scale <= 2500)
		      problem.task[i].setProfit( 1 + (int)Math.round((double)span * (50.0 + Math.sqrt((double)scale))));
		    else
		      problem.task[i].setProfit(1 + (int)Math.round((double)span * (300.0 - 2.0 * Math.sqrt((double)(12500 - scale)))));
		  }
		  Arrays.sort(problem.task);
	}
	public static void solve(Problem_T problem) {
		
	}
	
}
