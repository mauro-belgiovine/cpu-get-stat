#include "get_cpu_info.h"

//functions taken from Ivan Tikhonov
//procstat.c (2009) snippet

void readone(num *x, FILE *input) { fscanf(input, "%lld ", x); }
void readunsigned(unsigned long long *x, FILE *input) { fscanf(input, "%llu ", x); }
void readstr(char *x, FILE *input) {  fscanf(input, "%s ", x);}
void readchar(char *x, FILE *input) {  fscanf(input, "%c ", x);}

//return position index relative to the searched string in input file
void search_string_filestream(char *search, FILE *input, long *position){
  
  uint row_counter;
  char  tmp[256]={0x0};
  *position = NULL;
  
  while(input!=NULL && fgets(tmp, sizeof(tmp),input)!=NULL){
    if (strstr(tmp, search))
      //count current line length (plus \n character) and 
      //substract it from the file cursor position
      *position = ftell(input)-(strlen(tmp)+1);
  }
  
  rewind(input);
  
}

pid_stat_fields populate_pid_stats(FILE *stat_file){
    
  pid_stat_fields stats;
  //reading order is important!!!
  readone(&(stats.pid), stat_file);
  readstr(&(stats.tcomm), stat_file);
  readchar(&(stats.state), stat_file);
  readone(&(stats.ppid), stat_file);
  readone(&(stats.pgid), stat_file);
  readone(&(stats.sid), stat_file);
  readone(&(stats.tty_nr), stat_file);
  readone(&(stats.tty_pgrp), stat_file);
  readone(&(stats.flags), stat_file);
  readone(&(stats.min_flt), stat_file);
  readone(&(stats.cmin_flt), stat_file);
  readone(&(stats.maj_flt), stat_file);
  readone(&(stats.cmaj_flt), stat_file);
  readone(&(stats.utime), stat_file);
  readone(&(stats.stime), stat_file);
  readone(&(stats.cutime), stat_file);
  readone(&(stats.cstime), stat_file);
  readone(&(stats.priority), stat_file);
  readone(&(stats.nicev), stat_file);
  readone(&(stats.num_threads), stat_file);
  readone(&(stats.it_real_value), stat_file);
  readunsigned(&(stats.start_time), stat_file);
  readone(&(stats.vsize), stat_file);
  readone(&(stats.rss), stat_file);
  readone(&(stats.rsslim), stat_file);
  readone(&(stats.start_code), stat_file);
  readone(&(stats.end_code), stat_file);
  readone(&(stats.start_stack), stat_file);
  readone(&(stats.esp), stat_file);
  readone(&(stats.eip), stat_file);
  readone(&(stats.pending), stat_file);
  readone(&(stats.blocked), stat_file);
  readone(&(stats.sigign), stat_file);
  readone(&(stats.sigcatch), stat_file);
  readone(&(stats.wchan), stat_file);
  readone(&(stats.zero1), stat_file);
  readone(&(stats.zero2), stat_file);
  readone(&(stats.exit_signal), stat_file);
  readone(&(stats.cpu), stat_file);
  readone(&(stats.rt_priority), stat_file);
  readone(&(stats.policy), stat_file);
  
  return stats;
    
}

cpu_stat_fields populate_cpu_stats(FILE *stat_file, int cpu_num)
{
  cpu_stat_fields stats;
  long pos_index = 0;
  char cpu_label[10] = "\0"; //eg "cpu0", "cpu16"

  //if a cpu number (cpu_num) is specified,
  //we must collect stats for a specific #core
  
  if(cpu_num != ALL_CPUS){
    sprintf(&cpu_label, "cpu%d", cpu_num);
    
    search_string_filestream((char *) &cpu_label, stat_file, (long *) &pos_index);
    
    if(pos_index != NULL) fseek(stat_file, pos_index, SEEK_SET);
    //if something goes wrong here, we just read the first line
    else printf("%s not found... Monitoring cumulative CPU (ALL_CPUS)\n", cpu_label);
    
  }
  
  //otherwise, we read only cumulative cpu usage 
  //(first line of /proc/stat)
  
  readstr(&(stats.cpu_label), stat_file);
  readone(&(stats.user), stat_file);
  readone(&(stats.nice), stat_file);
  readone(&(stats.system), stat_file);
  readone(&(stats.idle), stat_file);
  readone(&(stats.iowait), stat_file);
  readone(&(stats.irq), stat_file);
  readone(&(stats.softirq), stat_file);
  
  return stats;
}

cpu_stat_fields get_cpu_stat_monitor(int cpu_num)
{
  FILE *fp;
  cpu_stat_fields stats;
  char buf[256];
  
  fp = fopen("/proc/stat", "r");
  if(fp != NULL){
    //read stat file and retreive infos
    stats = populate_cpu_stats(fp, cpu_num);
    //printf("cpustats.label %s\n", stats.cpu_label);
    //printf("cpustats.user %d\n", stats.user);
    //printf("cpustats.system %d\n", stats.system);
  }else{
    printf("file not found. Aborting... \n");
  }
  fclose( fp );
  
  return stats;
  
}

pid_stat_fields get_pid_stat_monitor()
{
  pid_t pid = getpid();
  FILE *fp;
  pid_stat_fields stats;
  char filename[100], buf[256];
    
  sprintf(filename, "/proc/%d/stat", pid);
  
  //apriamo il file
  fp = fopen(filename, "r");

  if(fp != NULL){
    //read stat file and retreive infos
    stats = populate_pid_stats(fp);
  }else{
    printf("file not found. Aborting... \n");
  }
  fclose( fp );
  
  return stats;
  
}



float pid_cpu_usage_percent(pid_stat_fields pid_start, pid_stat_fields pid_stop, 
			    cpu_stat_fields cpu_start, cpu_stat_fields cpu_stop){
  
  long tickspersec = sysconf(_SC_CLK_TCK);
    
  //You need to take the sum of the values on this line 
  //(/proc/stat cpu# interested line)
  //to get a time_total measure.
  long time_total_start, time_total_stop;
  float user_util, sys_util;
  
  time_total_start = cpu_start.user + cpu_start.system + cpu_start.nice + cpu_start.idle + cpu_start.iowait + cpu_start.irq + cpu_start.softirq;
  
  //Read both utime and stime for the process you're interested in, 
  //and read time_total from /proc/stat. Then sleep for a second or so,
  //and read them all again.
  
  time_total_stop = cpu_stop.user + cpu_stop.system + cpu_stop.nice + cpu_stop.idle + cpu_stop.iowait + cpu_stop.irq + cpu_stop.softirq;
  
  //get cpu usage percent for this particular process..
  //..using jiffies in user mode
  user_util = 100 * (pid_stop.utime - pid_start.utime) / (time_total_stop - time_total_start);
  //..using jiffies in kernel mode
  sys_util = 100 * (pid_stop.stime - pid_start.stime) / (time_total_stop - time_total_start);
  
  
  //return cumulative %
  return (user_util + sys_util);
    

}
