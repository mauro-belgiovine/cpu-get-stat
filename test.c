#include "get_cpu_info.h"

int main( int argc, char *argv[] )
{
  int i, n, x;
  time_t t;

  // -------------------------
  // --- ARGUMENT CHECKING ---
  // -------------------------

  int index;
  int c;

  int cpu_number = ALL_CPUS;
     
  opterr = 0;
     
  while ((c = getopt (argc, argv, "abc:")) != -1)
    switch (c)
    {
      case 'c':
        cpu_number = atoi(optarg);
        break;

      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,"Unknown option character `\\x%x'.\n", optopt);
        return 1;

      default:
        printf("Aborting...\n");
        abort ();
    }
          
  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  
  pid_stat_fields pid_start, pid_stop;
  cpu_stat_fields cpu_start, cpu_stop;
  
  pid_start = get_pid_stat_monitor();
  cpu_start = get_cpu_stat_monitor(cpu_number);
  
  printf("Start working stuff... cpu number: %d\n", cpu_number);
   
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
  cpu_stop = get_cpu_stat_monitor(cpu_number);
  
  float percent_usage = pid_cpu_usage_percent(pid_start, pid_stop, cpu_start, cpu_stop);
  
  printf("total cpu usage of this process: %f %% \n", percent_usage);
  
  return 0;
}

