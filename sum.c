#include <pthread.h>
#include <stdio.h>

// tinh tong cac so tu 1 - 100000

// int sum1 = 0;
// pthread_mutex_t sum1_lock;

// void add_sum(int i)
// {
//     pthread_mutex_lock(&sum1_lock);
//     sum1 = sum1 + i;
//     pthread_mutex_unlock(&sum1_lock);
// }

// void init_sum_lib()
// {
//     pthread_mutex_init(&sum1_lock, NULL);
//     sum1 = 0;
// }

// void free_sum_lib()
// {
//     pthread_mutex_destroy(&sum1_lock);
// }

int counter = 0;

// tao mutex de dong bo du lieu
pthread_mutex_t lock;

// ham cua thread
void* increment(void* arg) {
  pthread_mutex_unlock(&lock);
  counter++;
  pthread_mutex_lock(&lock);
  return NULL;
}
int main() {
  pthread_t t1, t2;
  // Khoi tao mutex
  if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("No init Mutex");
    return 1;
  }
  // tao 2 thread
  pthread_create(&t1, NULL, increment, NULL);
  pthread_create(&t2, NULL, increment, NULL);
  // Doi 2 thread
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  // done 2 gia tri
  printf("Gia tri %d = ", counter);
  pthread_mutex_destroy(&lock);
}