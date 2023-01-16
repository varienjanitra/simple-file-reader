#include <stdio.h>
#include <stdlib.h>

/*
* A function to load a file to memory
* This function takes a file name string as argument, and returns a pointer to char
*/
char *load_file_to_memory(char *file_name)
{
	/* Declare a temporary pointer variable to hold the pointer of memory which the file will be loaded to */
	char *file_in_memory_pointer = NULL;

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
	file_in_memory_pointer = malloc(file_size + 1);

	/* Block of code to load the file stream to the memory */
	char char_buffer = NULL;
	int i = 0;
	/* Keep reading the file stream until EOF is reached */
	while ((char_buffer = fgetc(file)) != EOF) {
		file_in_memory_pointer[i] = char_buffer;
		++i;
	}

	/* DO NOT forget to put null termination char '\0' at the end of the array! */
	file_in_memory_pointer[file_size] = '\0';

	/* DO NOT forget to close the file stream! */
	fclose(file);

	/* Return the file memory pointer */
	return file_in_memory_pointer;
}

int main()
{
	/* Load text file to memory */
	char *file = load_file_to_memory("test.txt");

	/* If file stream reading is failed, terminate the program */
	if(file == NULL)
		return EXIT_FAILURE;

	/* Show contents of file */
	printf("%s\n", file);

	/* DO NOT forget to free the text memory! */
	free(file);

	return EXIT_SUCCESS;
}