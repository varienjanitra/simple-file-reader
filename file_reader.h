#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef FILE_READER_H
#define FILE_READER_H

struct file_inmemory_t *file_load(char *file_name);
char *file_load_array(char *file_name);
void file_close(struct file_inmemory_t *file);

/* Representation of file inside the memory, abbreviated as FIM */
struct file_inmemory_t {
	struct file_inmemory_line_t *line;
	size_t total_lines;
};

/* Representation of a line inside the memory */
struct file_inmemory_line_t {
	char *line_data;
	size_t length;
	size_t line_number;
};

#endif