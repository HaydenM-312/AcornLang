#pragma once

#include <stdlib.h>
#include "lexer.h"

typedef struct __abstract_syntax_tree {
	struct __abstract_syntax_tree* 
			children;
	token	node;
	int 	child_count;
} ast_node;

typedef struct __context_tree {
	struct __context_tree* 
			children;
	size_t 	size;

	int 	type;
	void* 	data;

	int 	line;
	int 	column;
} context_tree;

	ast_node		
		ast_generate_node	(token);
void	ast_append_node		(ast_node*, ast_node);
void	ast_change_node		(ast_node*, int, ast_node);
void	ast_remove_node		(ast_node*, int);
void	ast_free_node		(ast_node*);

	context_tree	
		ctr_new_node		(int, void*, int, int);
void	ctr_append_node		(context_tree*, context_tree);
void	ctr_free_node		(context_tree*);