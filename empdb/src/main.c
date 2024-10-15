#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
  printf("Usage: %s -n -f <database file>\n", argv[0]);
  printf("\t -a  - Add new employee to database\n");
  printf("\t -n  - Create new database file\n");
  printf("\t -f  - (required) Path to database file\n");
  printf("\t -l  - List employee in database.\n");
}

int main(int argc, char * argv[]) {
  char *filepath = NULL;
  char *addRecord = NULL;
  bool newfile = false;
  bool list = false;
  int c;
  int dbfd = -1;
  struct dbheader_t *header = NULL;
  struct employee_t *employees = NULL;

  while((c = getopt(argc, argv, "nlf:a:")) != -1) {
      switch(c) {
        case 'a':
          addRecord = optarg;
          break;
        case 'n':
          newfile = true;
          break;
        case 'f':
          filepath = optarg;
          break;
        case 'l':
          list = true;
          break;

        case '?':
          printf("Unknown option -%c\n", c);
          break;
    }
  }

  if (filepath == NULL) {
      printf("Filepath is a required argument\n");
      print_usage(argv);
  }

  if(newfile) {
    printf("Creating new DB file at: %s\n", filepath);
    dbfd = create_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Unable to create database file\n");
      return STATUS_ERROR;
    }

    if (create_db_header(dbfd, &header) == STATUS_ERROR) {
      printf("Failed to create database header\n");
      return STATUS_ERROR;
    }
  } else {
    printf("Opening existing db: %s\n", filepath);
    dbfd = open_db(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Unable to open database file\n");
      return STATUS_ERROR;
    }

    printf("Database opened at[%d], path: %s\n", dbfd, filepath);
    if(validate_db_header(dbfd, &header) == STATUS_ERROR) {
      printf("Failed to validate database header\n");
      return STATUS_ERROR;
    }
  }

  printf("Newfile: %d\n", newfile);
  printf("Filepath: %s\n", filepath);

  if(read_employees(dbfd, header, &employees) != STATUS_SUCCESS) {
    printf("Read employees failed\n");
    return STATUS_ERROR;
  }

  if(addRecord) {
    header->count++;
    employees = realloc(employees, header->count * sizeof(struct employee_t));
    if(add_employee(header, employees, addRecord) != STATUS_SUCCESS) {
      printf("Adding employee failed.\n");
      return STATUS_ERROR;
    }
  }

  if(list) {
    list_employees(header, employees);
  }

  if (addRecord || newfile) {
    output_file(dbfd, header, employees);
  }

  return 0;
}
