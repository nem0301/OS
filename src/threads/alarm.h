#ifndef THREADS_ALARM_H
#define THREADS_ALARM_H

#include <list.h>
#include <debug.h>
#include "threads/interrupt.h"
#include "devices/timer.h"
#include "threads/thread.h"
#include <stdio.h>
#include <stdint.h>

struct alarm {
  int64_t expiration;
  struct thread* th;
  struct list_elem elem;
};

void alarm_init (void);
void alarm_insert (int64_t tick);
void alarm_check(void);

#endif
