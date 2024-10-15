#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "common.h"
#include "file.h"

int guard_file_exists(char *filename) {
  int fd = open(filename, O_RDONLY);
  if(fd != -1) {
    close(fd);
    return STATUS_ERROR;
  }

  return STATUS_SUCCESS;
}

int create_db_file(char *filename) {
  if(guard_file_exists(filename) != STATUS_SUCCESS) {
    printf("File already exists!!!\n");
    return STATUS_ERROR;
  }

  int fd = open(filename, O_RDWR | O_CREAT, 0644);

  if(fd == -1) {
    perror("open");
    return STATUS_ERROR;
  }
  return fd;
}

int open_db(char *filename) {
 int fd = open(filename, O_RDWR, 0644);

  if(fd == -1) {
    perror("open");
    return STATUS_ERROR;
  }
  return fd;
}



