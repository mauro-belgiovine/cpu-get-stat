/*
 * Functions to get stat information about cpu and/or processes
 * reading from /proc/stat and /proc/<pid>/stat files
 * Based on a concept explained in question from:
 * http://stackoverflow.com/questions/1420426/calculating-cpu-usage-of-a-process-in-linux/1424556#1424556
 *
 * Homepage: ...
 * Version : 2013-10-15
 *
 * Mauro Belgiovine - geek89@gmail.com, mauro.belgiovine@bo.infn.it
 * 
 *
 */

/* Copyright (C) 2013 Mauro Belgiovine	

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Mauro Belgiovine - geek89@gmail.com, mauro.belgiovine@bo.infn.it

*/

#pragma once
//#pragma once is a non-standard but widely supported preprocessor directive designed 
//to cause the current source file to be included only once in a single compilation

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <linux/limits.h>
#include <sys/times.h>

#define ALL_CPUS -1

typedef long long int num;

typedef struct pid_stat_fields{
  
  num pid;
  char tcomm[PATH_MAX];
  char state;

  num ppid;
  num pgid;
  num sid;
  num tty_nr;
  num tty_pgrp;

  num flags;
  num min_flt;
  num cmin_flt;
  num maj_flt;
  num cmaj_flt;
  num utime;
  num stime;

  num cutime;
  num cstime;
  num priority;
  num nicev;
  num num_threads;
  num it_real_value;

  unsigned long long start_time;

  num vsize;
  num rss;
  num rsslim;
  num start_code;
  num end_code;
  num start_stack;
  num esp;
  num eip;

  num pending;
  num blocked;
  num sigign;
  num sigcatch;
  num wchan;
  num zero1;
  num zero2;
  num exit_signal;
  num cpu;
  num rt_priority;
  num policy;
  
}pid_stat_fields;

//TODO: extend struct to all fields of /proc/state;
//only CPU's jiffies are tracked
typedef struct cpu_stat_fields{
  char cpu_label[10];
  num user;
  num nice;
  num system;
  num idle;
  num iowait;
  num irq;
  num softirq;
  /* ... */
}cpu_stat_fields;

#ifdef __cplusplus
extern "C" {
#endif
  //utilities
  void readone(num *x, FILE *input);
  void readunsigned(unsigned long long *x, FILE *input);
  void readstr(char *x, FILE *input);
  void readchar(char *x, FILE *input);
  
  //NOTE: these functions assume that input FILE *stat_file is already opened
  void search_string_filestream(char *search, FILE *input, long *position);
  pid_stat_fields populate_pid_stats(FILE *stat_file);
  cpu_stat_fields populate_cpu_stats(FILE *stat_file, int cpu_num);
  
  //functions
  pid_stat_fields get_pid_stat_monitor();
  cpu_stat_fields get_cpu_stat_monitor(int cpu_num);
  //NOTE: this functions collects data only concerning parent process (not his children)
  float pid_cpu_usage_percent(pid_stat_fields pid_start, pid_stat_fields pid_stop, 
	cpu_stat_fields cpu_start, cpu_stat_fields cpu_stop);
    
#ifdef __cplusplus
}
#endif
