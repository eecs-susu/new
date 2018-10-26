#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

int main() {
  const char* name = "/tmp/shared-me";

  if (mkfifo(name, 0666) == -1) {
    printf("Warning: %s\n", strerror(errno));
  }

  int descriptor = open(name, O_WRONLY);
  if (descriptor == -1) {
    printf("Unable to get descriptor: %s\n", strerror(errno));
    exit(1);
  }
  const char* message = "Hello";
  const char* stop_message = "Bye";
  for (int i = 0; i < 10; ++i) {
    if (write(descriptor, message, strlen(message) + 1) == -1) {
      printf("Unable to send message to fifo file: %s\n", strerror(errno));
      exit(1);
    }
    printf("Sent %d\n", i);
    usleep(1000000);
  }
  if (write(descriptor, stop_message, strlen(stop_message) + 1) == -1) {
    printf("Unable to send message to fifo file: %s\n", strerror(errno));
    exit(1);
  }
  if (close(descriptor) == -1) {
    printf("Close failed: %s\n", strerror(errno));
    exit(1);
  }
  return 0;
}