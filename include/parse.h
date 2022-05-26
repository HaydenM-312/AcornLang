#pragma once

#include <stdio.h>
#include "lexer.h"
#include "astnode.h"

ast_node	parse			(token_list);
ast_node 	parse_token		(ast_node*, ast_node, int*, ast_node**);
ast_node	parse_expr		(ast_node);
int			is_higher_pres	(int, int);

char*		name_token		(int);
void		print_token		(token);
void		print_ast		(ast_node);