#include "file_reader.h"

#define FILE_LOADING_MEMORY_BUFFER 128

void file_load_to_memory(FILE *file, struct file_inmemory_t *file_inmemory);
size_t file_count_total_line(FILE *file);

struct file_inmemory_t *file_load(char *file_name)
{
	/* Allocate memory for the in-memory file representation */
	struct file_inmemory_t *file_inmemory = malloc(sizeof(struct file_inmemory_t));
	if (file_inmemory == NULL) {
		printf("Error in allocating memory for in-memory file representation!\n");
		return NULL;
	}

	/* Open the file stream in read mode */
	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		printf("Error in loading file!\n");
		return NULL;
	}

	/* Count total length of the file and allocate memory with corresponding size */
	size_t total_line = file_count_total_line(file);
	file_inmemory->line = malloc(total_line * sizeof(struct file_inmemory_line_t));
	if (file_inmemory == NULL) {
		printf("Error in allocating memory for in-memory file representation!\n");
		return NULL;
	}

	/* Set total line to counted value */
	file_inmemory->total_lines = total_line;

	/* Load the file to memory */
	file_load_to_memory(file, file_inmemory);

	/* Return the pointer of in-memory file representation */
	return file_inmemory;
}

char *file_load_array(char *file_name)
{
	/* Declare a temporary pointer variable to hold the pointer of memory which the file will be loaded to */
	char *file_inmemory = NULL;

	/* Open the file stream in read mode */
	FILE *file = fopen(file_name, "r");

	/* Return NULL pointer if the file stream read is failed */
	if (file == NULL) {
		printf("Error in loading file!\n");
		return NULL;
	}

	/* Calculate file stream length (total characters) */
	/* Move the file stream pointer to the end of stream to find the length*/
	fseek(file, 0, SEEK_END);

	/* file_size is in bytes (total characters * size of character in bytes) for memory allocation purpose*/
	size_t file_size = ftell(file) * sizeof(char);

	/* DO NOT forget to rewind the file stream pointer so the subsequent reading could be done */
	rewind(file);

	/* Allocate memory of corresponding size PLUS 1 to account for null termination char '\0' */
	file_inmemory = malloc(file_size + 1);

	/* Block of code to load the file stream to the memory */
	int char_buffer = 0;
	int i = 0;

	/* Keep reading the file stream until EOF is reached */
	while ((char_buffer = fgetc(file)) != EOF) {
		file_inmemory[i] = char_buffer;
		++i;
	}

	/* DO NOT forget to put null termination char '\0' at the end of the array! */
	file_inmemory[file_size] = '\0';

	/* DO NOT forget to close the file stream! */
	fclose(file);

	/* Return the file memory pointer */
	return file_inmemory;
}

void file_close(struct file_inmemory_t *file)
{
	/* Free the memory allocated for line data */
	for (size_t line = 0; line < file->total_lines; ++line) {
		free((file->line[line]).line_data);
	}

	/* Free the memory allocated for the in-memory line representation  */
	free(file->line);

	/* Free the memory allocated for the in-memory file representation */
	free(file);
}

void file_load_to_memory(FILE *file, struct file_inmemory_t *file_inmemory)
{
	/* Initialize all counters and buffers */
	int char_buffer = 0;
	size_t current_line = 0;
	size_t character_counter = 0;
	
	/* Allocate memory for line buffer */
	char *line_buffer = malloc(FILE_LOADING_MEMORY_BUFFER * sizeof(char));
	size_t line_buffer_size = FILE_LOADING_MEMORY_BUFFER;
	const unsigned char line_buffer_size_increment = FILE_LOADING_MEMORY_BUFFER;

	/* Read the file stream until EOF is reached */
	while (char_buffer != EOF) {
		/* Read current character pointed by file stream */
		char_buffer = fgetc(file);

		/* Allocate more memory for line buffer when the line length is larger than the buffer */
		if (character_counter >= line_buffer_size) {
			line_buffer = realloc(line_buffer, (line_buffer_size + line_buffer_size_increment) * sizeof(char));
			line_buffer_size += line_buffer_size_increment;
		}

		/* 
		* When the current character is either newline or EOF,
		* transfer the contents of line buffer to the data container of in-memory line representation 
		*/
		if (char_buffer == '\n' || char_buffer == EOF) {
			/* Append 'null' character at the end of line */
			line_buffer[character_counter] = '\0';

			/* Allocate memory for the data inside the in-memory line representation */
			(file_inmemory->line[current_line]).line_data = malloc((character_counter + 1) * sizeof(char));

			/* Transfer the contents of line buffer to the data container */
			strncpy((file_inmemory->line[current_line]).line_data, line_buffer, (character_counter + 1));

			/* Set line length to counted value */
			(file_inmemory->line[current_line]).length = character_counter;

			/* Reset character counter */
			character_counter = 0;

			/* Set line number to counted value */
			(file_inmemory->line[current_line]).line_number = current_line + 1;

			/* Advance line counter */
			++current_line;

			/* Continue reading next character */
			continue;
		}
		
		/* If the characters is other than newline or EOF, put it inside the line buffer */
		/* These lines are put below so that the logic would not mess with the C's array element numbering */
		line_buffer[character_counter] = char_buffer;

		/* Advance the line counter */
		++character_counter;

	}

	/* Do NOT forget to free the line buffer */
	free(line_buffer);

	/* DO NOT forget to close the file stream! */
	fclose(file);

	/* 
	* The file_inmemory pointer is automatically updated,
	* because it is a pointer passed on the function argument, therefore no return values.
	*/
}

size_t file_count_total_line(FILE *file)
{
	size_t total_lines = 0;
	int char_buffer = 0;

	/* Find all the newline and EOF characters, until EOF */
	while(char_buffer != EOF) {
		char_buffer = fgetc(file);

		/* Count every encounter of newline and EOF characters */
		if (char_buffer == '\n' || char_buffer == EOF)
			++total_lines;
	}

	/* 
	* DO NOT forget to rewind the file, 
	* else any subsequent file stream reading would fail!
	*/
	rewind(file);

	return total_lines;
}