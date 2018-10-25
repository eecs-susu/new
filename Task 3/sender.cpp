#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
  const char *name = "/shared-mem";
  const int mem_size = 4096;

  int descriptor = shm_open(name, O_CREAT | O_RDWR, 0666);
  if (descriptor == -1) {
    printf("prod: Shared memory failed: %s\n", strerror(errno));
    exit(1);
  }

  ftruncate(descriptor, mem_size);

  void *shared_addr;
  shared_addr =
      mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor, 0);

  if (shared_addr == MAP_FAILED) {
    printf("prod: Map failed: %s\n", strerror(errno));
    exit(1);
  }

  char *text = new char[mem_size];
  printf("Enter message: ");
  scanf("%[^\n]", text);
  char *writer = reinterpret_cast<char *>(shared_addr);
  writer += snprintf(writer, mem_size - 1, "%s", text);

  if (munmap(shared_addr, mem_size) == -1) {
    printf("prod: Unmap failed: %s\n", strerror(errno));
    exit(1);
  }

  if (close(descriptor) == -1) {
    printf("prod: Close failed: %s\n", strerror(errno));
    exit(1);
  }

  return 0;
}
