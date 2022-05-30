#include "astnode.h"

ast_node generate_node(token t) {
	ast_node AST;
	AST.node = t;
	AST.children = NULL;
	AST.child_count = 0;

	return AST;
}

void append_node(ast_node* this, ast_node node) {
	this->children = realloc(this->children, ++(this->child_count) * sizeof(ast_node));
	if (this->children == NULL) error(2, NULL);
	this->children[this->child_count - 1] = node;
}

void change_node(ast_node* this, int position, ast_node new_node) {
	this->children[position] = new_node;
}

void remove_node(ast_node* this, int position) {
	for (int i = position + 1; i < this->child_count; i++) this->children[i-1] = this->children[i];
	this->children = realloc(this->children, --(this->child_count) * sizeof(ast_node));
}

void free_node(ast_node* this) {
	if (this->node.data != NULL) free(this->node.data);
	if (this->children != NULL) {
		for (int i = 0; i < this->child_count; i++) free_node(&(this->children[i]));
	}
}
