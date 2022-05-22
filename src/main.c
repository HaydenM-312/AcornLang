#include "main.h"

int main(int argc, char* argv[]) {
	char* file = fread_to_buffer(argv[1]);
	token_list* tokenl = parse_to_token(file);
	ast_node AST = parse(*tokenl);

	print_ast(AST, 0);

	free_token_list(tokenl);
	free_node(&AST);
	free(file);
	
	return 0;
}