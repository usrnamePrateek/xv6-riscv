#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
run_loop(char *name, int prio)
{
  set_priority(prio);

  for (int i = 0; i < 10; i++) {
    printf("%s (PID %d, PRIO %d) → ITER %d\n",
           name, getpid(), get_priority(), i);

    // burn CPU to give scheduler some work
    for (volatile int k = 0; k < 10000000000; k++);
  }

  exit(0);
}

int
main(void)
{
  int pid;

  // First child: low priority
  pid = fork();
  if (pid == 0) {
    run_loop("LOW", 5);   // low prio
  }

  // Second child: high priority
  pid = fork();
  if (pid == 0) {
    run_loop("HIGH", 20); // high prio
  }

  // Parent waits for both children
  wait(0);
  wait(0);

  exit(0);
}