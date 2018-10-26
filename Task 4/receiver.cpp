#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

int main() {
  const char* name = "/tmp/shared-me";
  const int buf_size = 4096;
  char* buffer = new char[buf_size];
  int descriptor = open(name, O_RDONLY);
  if (descriptor == -1) {
    printf("Unable to get descriptor: %s\n", strerror(errno));
    exit(1);
  }
  const char* stop_message = "Bye";
  while (true) {
    if (read(descriptor, buffer, buf_size - 1) == -1) {
      printf("Unable to receive message to fifo file: %s\n", strerror(errno));
      exit(1);
    }
    if (strcmp(buffer, stop_message) == 0) {
      break;
    }

    printf("Received message: %s\n", buffer);
  }
  if (close(descriptor) == -1) {
    printf("Close failed: %s\n", strerror(errno));
    exit(1);
  }
  if (unlink(name) == -1) {
    printf("Unable to unlink fifo file: %s\n", strerror(errno));
    exit(1);
  }
  return 0;
}