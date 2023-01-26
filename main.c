#include <stdio.h>
#include <stdlib.h>
#include "file_reader.h"

int main()
{
	/* Load text file to memory */
	char *file = file_load_array("test.txt");

	/* If file stream reading is failed, terminate the program */
	if(file == NULL)
		return EXIT_FAILURE;

	/* Show contents of file */
	printf("%s\n", file);

	/* DO NOT forget to free the text memory! */
	free(file);

	return EXIT_SUCCESS;
}