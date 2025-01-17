#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x526f6247 //RobG

struct dbheader_t {
  unsigned int magic;
  unsigned short version;
  unsigned short count;
  unsigned int filesize;
};

struct employee_t {
  char name[256];
  char address[256];
  unsigned int hours;
};

int add_employee(struct dbheader_t *header, struct employee_t *employees, char *addstring);
int del_employee(struct dbheader_t *header, struct employee_t *employees, char *delName);
int search_employee(int fd, struct dbheader_t *header, char *name, struct employee_t **employeesOut);
int create_db_header(int fd, struct dbheader_t **headerOut);
int validate_db_header(int fd, struct dbheader_t **headerOut);
int list_employees(struct dbheader_t *header, struct employee_t *employees);
int read_employees(int fd, struct dbheader_t *header, struct employee_t **employeesOut);
void output_file(int fd, struct dbheader_t *header, struct employee_t *employees);

#endif
