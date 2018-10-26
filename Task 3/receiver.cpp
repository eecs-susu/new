#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

int main(void) {
  const char *name = "/shared-mem";
  const int mem_size = 4096;

  int descriptor = shm_open(name, O_RDONLY, 0666);
  if (descriptor == -1) {
    printf("Shared memory failed: %s\n", strerror(errno));
    exit(1);
  }

  void *shared_addr = mmap(0, mem_size, PROT_READ, MAP_SHARED, descriptor, 0);
  if (shared_addr == MAP_FAILED) {
    printf("Map failed: %s\n", strerror(errno));
    exit(1);
  }

  printf("Awaiting for message...");
  while (reinterpret_cast<char *>(shared_addr)[0] == '\0') {
    printf("\b\b\b.  ");
    fflush(stdout);
    usleep(500000);
    printf("\b\b\b.. ");
    fflush(stdout);
    usleep(500000);
    printf("\b\b\b...");
    fflush(stdout);
    usleep(500000);
  }

  printf("\nReceived message: \"%s\"\n", shared_addr);

  if (munmap(shared_addr, mem_size) == -1) {
    printf("Unmap failed: %s\n", strerror(errno));
    exit(1);
  }

  if (close(descriptor) == -1) {
    printf("Close failed: %s\n", strerror(errno));
    exit(1);
  }

  if (shm_unlink(name) == -1) {
    printf("Error removing %s: %s\n", name, strerror(errno));
    exit(1);
  }

  return 0;
}
