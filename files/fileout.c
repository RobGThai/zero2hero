#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
  // Open FD
  int fd = open("test_file.txt", O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open");
    return -1;
  }

  // Write data to file
  char *data = "dummy data\n";
  write(fd, data, strlen(data));

  close(fd);

  return 0;
}
