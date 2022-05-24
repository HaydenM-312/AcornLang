#include "error.h"

void error(int err_no, void* data) {
	switch (err_no) {
		case 1:
			fprintf(stderr, "Error %i: Invalid file name %s", err_no, (char*) data);
			break;
		case 2:
			fprintf(stderr, "Error %i: Out of memory", err_no);
			break;
		case 3:
			fprintf(stderr, "Error %i: Unable to open file %s", err_no, (char*) data);
			break;
		case 4:
			fprintf(stderr, "Error %i: Incomplete string", err_no);
			break;
		case 5:
			fprintf(stderr, "Error %i: Unclosed parentheses at line %i, column %i", err_no, ((int*) data)[0], ((int*) data)[1]);
			break;
		default:
			fprintf(stderr, "Error %i: Unknown error", err_no);
			break;
	}
	
	exit(err_no);
}

void mark() {
	printf("\t[Marked]\n");
}