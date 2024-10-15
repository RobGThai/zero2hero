#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>

struct database_header {
    unsigned short version;
    unsigned short employees;
    unsigned int filelength;
};

int main() {
  int fd = open("./my-db.db", O_RDONLY);
  if (fd == -1) {
    perror("open");
    return -1;
  }

  struct database_header header = {0};

  int r = read(fd, &header, sizeof(header));
  if (r == -1) {
    perror("read");
    return -1;
  }
  printf("Database version %d\n", header.version);
  
  struct stat dbstat = {0};
  if (fstat(fd, &dbstat) < 0) {
    perror("fstat");
    close(fd);
    return -1;
  }

  printf("fstat: %ld\n", (long)dbstat.st_size);
  printf("fstat device id: %d\n", dbstat.st_dev);
  printf("fstat mode: %d\n", dbstat.st_mode);
  printf("fstat links #: %d\n", dbstat.st_nlink);
  printf("fstat uid: %d\n", dbstat.st_uid);
  
  if( (long) dbstat.st_size != header.filelength) {
    printf("File size mismatched!! %ld != %d", (long)dbstat.st_size, header.filelength);
    close(fd);
    return -1;
  }


  close(fd);

  return 0;
}
