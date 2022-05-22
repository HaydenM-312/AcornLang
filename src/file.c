#include "file.h"

char* fread_to_buffer(char* address) {
	FILE* fptr = fopen(address, "r");;
	char* contents;
	size_t length;
	size_t n_len;

	

	// Check for any errors
	if (fptr == NULL) error(1, address);

	// Allocate the size of the buffer for the file
	fseek(fptr, 0L, SEEK_END);
	length = ftell(fptr);
	contents = malloc(length+1);
	rewind(fptr);

	// Read the file into the buffer
	n_len = fread(contents, sizeof(char), length, fptr);

	// Check for errors (again)
	if (ferror(fptr) != 0)
		error(3, address);

	contents[n_len++] = '\0';

	fclose(fptr);
	return contents;
}