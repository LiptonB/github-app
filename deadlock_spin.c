#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct threadargs {
  pthread_spinlock_t *lock1;
  pthread_spinlock_t *lock2;
};

void *thread_func(void *arg) {
  struct threadargs *args = (struct threadargs *)arg;

  pthread_spin_lock(args->lock2);
  printf("lock2 held by subthread\n");

  pthread_spin_lock(args->lock1);
  printf("lock1 held by subthread\n");

  pthread_spin_unlock(args->lock2);
  pthread_spin_unlock(args->lock1);

  return NULL;
}

int main() {
  pthread_t thread;
  pthread_spinlock_t lock1;
  pthread_spinlock_t lock2;
  struct threadargs args;

  pthread_spin_init(&lock1, PTHREAD_PROCESS_PRIVATE);
  pthread_spin_init(&lock2, PTHREAD_PROCESS_PRIVATE);
  args.lock1 = &lock1;
  args.lock2 = &lock2;

  if(pthread_create(&thread, NULL, &thread_func, &args)) {
    perror("pthread_create");
    exit(1);
  }

  pthread_spin_lock(&lock1);
  printf("lock1 held by main thread\n");

  // Intentionally lose the race for lock2
  sleep(1);

  pthread_spin_lock(&lock2);
  printf("lock2 held by main thread\n");

  pthread_spin_unlock(&lock2);
  pthread_spin_unlock(&lock1);

  pthread_join(thread, NULL);
}
