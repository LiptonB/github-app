#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct threadargs {
  pthread_mutex_t *mutex1;
  pthread_mutex_t *mutex2;
};

void *thread_func(void *arg) {
  struct threadargs *args = (struct threadargs *)arg;

  pthread_mutex_lock(args->mutex2);
  printf("mutex2 held by subthread\n");

  pthread_mutex_lock(args->mutex1);
  printf("mutex1 held by subthread\n");

  pthread_mutex_unlock(args->mutex2);
  pthread_mutex_unlock(args->mutex1);

  return NULL;
}

int main() {
  pthread_t thread;
  pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
  struct threadargs args;

  args.mutex1 = &mutex1;
  args.mutex2 = &mutex2;

  if(pthread_create(&thread, NULL, &thread_func, &args)) {
    perror("pthread_create");
    exit(1);
  }

  pthread_mutex_lock(&mutex1);
  printf("mutex1 held by main thread\n");

  // Intentionally lose the race for mutex2
  sleep(1);

  pthread_mutex_lock(&mutex2);
  printf("mutex2 held by main thread\n");

  pthread_mutex_unlock(&mutex2);
  pthread_mutex_unlock(&mutex1);

  pthread_join(thread, NULL);
}
