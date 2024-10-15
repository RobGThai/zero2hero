#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "parse.h"
#include "common.h"

void to_host_endian(struct dbheader_t *header) {
  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->filesize = ntohl(header->filesize);
}

void to_network_endian(struct dbheader_t *header) {
  header->version = htons(header->version);
  header->count = htons(header->count);
  header->magic = htonl(header->magic);
  header->filesize = htonl(header->filesize);
}

int add_employee(struct dbheader_t *header, struct employee_t *employees, char *addRecord) {
  printf("Adding employee.\n");
  char *name = strtok(addRecord, ",");
  char *addr = strtok(NULL, ",");
  char *hours = strtok(NULL, ",");

  printf("%s - %s - %s\n", name, addr, hours);
  strncpy(employees[header->count -1].name, name, sizeof(employees[header->count -1].name));
  strncpy(employees[header->count -1].address, addr, sizeof(employees[header->count -1].address));
  employees[header->count -1].hours = atoi(hours);

  return STATUS_SUCCESS;
}

int create_db_header(int fd, struct dbheader_t **headerOut) {
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if(header == NULL) { // Tutorial use == -1
    printf("Malloc failed to create db header\n");
    return STATUS_ERROR;
  }

  header->version = 0x1;
  header->count = 0 ;
  header->magic = HEADER_MAGIC;
  header->filesize = sizeof(struct dbheader_t);

  *headerOut = header;

  return STATUS_SUCCESS;  
}

int list_employees(struct dbheader_t *header, struct employee_t *employees) {

  int i = 0;
  for(; i < header->count; i++) {
    printf("Employee: %d\n", i);
    printf("\tName: %s\n", employees[i].name);
    printf("\tAddress: %s\n", employees[i].address);
    printf("\tHours: %d\n", employees[i].hours);
  }

  return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t *header, struct employee_t **employeesOut) {
  if(fd < 0) {
    printf("Got a bad FD from the user\n");
  }

  int count = header->count;

  struct employee_t *employees = calloc(count, sizeof(struct employee_t));

  if(employees == NULL) {
    printf("Malloc failed to create db header\n");
    return STATUS_ERROR;
  }

  int targetSize = count * sizeof(struct employee_t);
  if(read(fd, employees, targetSize) != targetSize) {
    perror("read");
    free(employees);
    return STATUS_ERROR;
  }

  int i = 0;
  for(; i < count; i++) {
    // Handle endianess
    employees[i].hours = ntohl(employees[i].hours);
  }

  *employeesOut = employees;

  return STATUS_SUCCESS;
}

void output_file(int fd, struct dbheader_t *header, struct employee_t *employees) {
  printf("Output file FD at [%d]\n", fd);
  if(fd < 0) {
    printf("Got a bad FD from the user\n");
  }

  printf("Writing header version: %d\n", header->version);

  int totalRecord = header->count;
  header->filesize = sizeof(struct dbheader_t) + (totalRecord * sizeof(struct employee_t));

  to_network_endian(header);

  printf("Reset cursor to the start\n");
  // Move cursor of this fd to the beginning (offset 0)
  lseek(fd, 0, SEEK_SET);

  printf("Writing header content to file.\n");
  // Overwrite existing header
  write(fd, header, sizeof(struct dbheader_t));
  
  int i = 0;
  for(; i < totalRecord; i++) {
    employees[i].hours = htonl(employees[i].hours);
    write(fd, &employees[i], sizeof(struct employee_t));
  }
}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
  printf("Validating FD at [%d]\n", fd);
  if(fd < 0) {
    printf("Got a bad FD from the user\n");
    return STATUS_ERROR;
  }

  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if(header == NULL) {
    printf("Malloc failed to create db header\n");
    return STATUS_ERROR;
  }

  printf("Attempting to read header\n");
  if(read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
    printf("Error reading header file \n");
    perror("read");
    free(header);
    return STATUS_ERROR;
  }

  printf("Validating content\n");
  to_host_endian(header);

  if(header->magic != HEADER_MAGIC) {
    printf("Improper header magic\n");
    free(header);
    return STATUS_ERROR;
  }

  if(header->version != 1) {
    printf("Improper header version\n");
    free(header);
    return STATUS_ERROR;
  }

  struct stat dbstat = {0};
  fstat(fd, &dbstat);

  if(header->filesize != dbstat.st_size) {
    printf("Corrupted database\n");
    free(header);
    return STATUS_ERROR;
  }

  *headerOut = header;

  return STATUS_SUCCESS;
}

