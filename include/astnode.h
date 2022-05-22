#pragma once

#include <stdlib.h>
#include "lexer.h"

typedef struct __abstract_syntax_tree {
	token node;
	struct __abstract_syntax_tree* children;
	int child_count;
} ast_node;

ast_node	generate_node	(token);
void		append_node		(ast_node*, ast_node);
void		change_node		(ast_node*, int, ast_node);
void		remove_node		(ast_node*, int);
void		free_node		(ast_node*);