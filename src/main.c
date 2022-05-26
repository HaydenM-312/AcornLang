#include "main.h"
#include <Windows.h>

int main(int argc, char* argv[]) {
	SetConsoleOutputCP(CP_UTF8);
	if (argc > 1) {	
		char* file = fread_to_buffer(argv[1]);
		token_list* tokenl = parse_to_token(file);
		ast_node AST = parse(*tokenl);

		print_ast(AST);

		free(file);
		free_node(&AST);
		free_token_list(tokenl);
	} else {
		while (1) {
			char input[256];
			printf("Acorn> ");
			fgets(input, 256, stdin);

			token_list* tokenl = parse_to_token(input);
			ast_node AST = parse(*tokenl);

			print_ast(AST);

			free_node(&AST);
			free_token_list(tokenl);
		}
	}
	
	return 0;
}