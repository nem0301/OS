#include "threads/alarm.h"
#include "threads/malloc.h"

static struct list alarms;
static bool alarm_sort(const struct list_elem* a, const struct list_elem* b, void* aux UNUSED);

void alarm_init (void) {
  list_init(&alarms);
}

void alarm_insert(int64_t tick) {
  struct thread* cur = thread_current();
  struct alarm* al;
  enum intr_level old_level;

  ASSERT(cur->status == THREAD_RUNNING);

  old_level = intr_disable();

  //printf("%s %d\n", thread_current()->name, (int)tick);

  al = (struct alarm*) malloc (sizeof (struct alarm));
  al->expiration = tick;
  al->th = cur;


  list_push_back(&alarms, &al->elem);
  list_sort(&alarms, alarm_sort, NULL);

  thread_block();

  intr_set_level(old_level);
}

void alarm_check(void) {
  struct alarm* al;
  enum intr_level old_level;


  old_level = intr_disable();

  while (!list_empty(&alarms)
      && (al = list_entry(list_begin(&alarms), struct alarm, elem))->expiration
          <= timer_ticks()) {
    list_remove(&al->elem);
    thread_unblock(al->th);
  }

  intr_set_level(old_level);

}

static bool alarm_sort(const struct list_elem* a, const struct list_elem* b, void* aux UNUSED) {
  struct alarm* al1;
  struct alarm* al2;

  ASSERT(a != NULL && b != NULL);

  al1 = list_entry(a, struct alarm, elem);
  al2 = list_entry(b, struct alarm, elem);

  return (al1->expiration < al2->expiration);

}
