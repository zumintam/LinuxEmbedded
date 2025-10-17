#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Tạo mutex để đồng bộ dữ liệu
pthread_mutex_t lock;

// Hàm wrapper bảo vệ thao tác ghi
void safe_write(int fd, const char *buf) {
  pthread_mutex_lock(&lock);    // 🔒 khóa
  write(fd, buf, strlen(buf));  // ghi dữ liệu
  pthread_mutex_unlock(&lock);  // 🔓 mở khóa
}

// Hàm cho thread
void *thread_func(void *arg) {
  int fd = *(int *)arg;
  for (int i = 0; i < 10; i++) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "Thread %lu: line %d\n", pthread_self(),
             i + 1);
    safe_write(fd, buffer);
    usleep(100000);  // ngủ 0.1 giây để xen kẽ ghi
  }
  return NULL;
}

int main() {
  pthread_t t1, t2;

  // Khởi tạo mutex
  if (pthread_mutex_init(&lock, NULL) != 0) {
    perror("pthread_mutex_init");
    return 1;
  }

  // Mở file (kiểu fd)
  int fd = open("/mnt/shared/Downloads/shared_lib/src/text.txt",
                O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    perror("open");
    return 1;
  }

  // Tạo 2 thread ghi vào file
  pthread_create(&t1, NULL, thread_func, &fd);
  pthread_create(&t2, NULL, thread_func, &fd);

  // Chờ 2 thread kết thúc
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  // Dọn dẹp
  close(fd);
  pthread_mutex_destroy(&lock);

  printf("✅ Ghi file thành công: text.txt\n");
  return 0;
}
