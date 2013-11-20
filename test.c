#include "get_cpu_info.h"

int main( int argc, char *argv[] )
{
  int i, n, x;
  time_t t;
  
  pid_stat_fields pid_start, pid_stop;
  cpu_stat_fields cpu_start, cpu_stop;
  
  pid_start = get_pid_stat_monitor();
  cpu_start = get_cpu_stat_monitor(ALL_CPUS);
  
  printf("Start working stuff...\n");
   
  //RANDOM STUFF TO MAKE CPU WORK
  
  n = 1000000000;
   
  /* Intializes random number generator */
  srand((unsigned) time(&t));

  // Print 5 random numbers from 0 to 50
  for( i = 0 ; i < n ; i++ ) {
      x = rand() % 50;
      x = x*x;
      //printf("%ld ", x);
      
  }
  printf("\n");
  
  pid_stop = get_pid_stat_monitor();
  cpu_stop = get_cpu_stat_monitor(ALL_CPUS);
  
  float percent_usage = pid_cpu_usage_percent(pid_start, pid_stop, cpu_start, cpu_stop);
  
  printf("total cpu usage of this process: %f %% \n", percent_usage);
  
  return 0;
}

