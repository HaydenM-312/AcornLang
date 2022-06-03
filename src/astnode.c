#include "astnode.h"
#include "lexer.h"

ast_node ast_generate_node(token t) {
	ast_node AST;
	AST.node = t;
	AST.children = NULL;
	AST.child_count = 0;

	return AST;
}

void ast_append_node(ast_node* this, ast_node node) {
	this->children = realloc(this->children, ++(this->child_count) * sizeof(ast_node));
	if (this->children == NULL) error(2, NULL);
	this->children[this->child_count - 1] = node;
}

void ast_change_node(ast_node* this, int position, ast_node new_node) {
	this->children[position] = new_node;
}

void ast_remove_node(ast_node* this, int position) {
	for (int i = position + 1; i < this->child_count; i++) this->children[i-1] = this->children[i];
	this->children = realloc(this->children, --(this->child_count) * sizeof(ast_node));
}

void ast_free_node(ast_node* this) {
	if (this->node.data != NULL) free(this->node.data);
	if (this->children != NULL) {
		for (int i = 0; i < this->child_count; i++) ast_free_node(&(this->children[i]));
	}
	free(this->children);
}


context_tree ctr_new_node(int _type, void* _data, int _line, int _column) {
	context_tree tree;

	tree.type = _type;
	tree.data = _data;
	tree.line = _line;
	tree.column = _column;

	tree.data = _data;
	tree.children = malloc(0);
	tree.size = 0;

	return tree;
}

void ctr_append_node(context_tree* this, context_tree new) {
	this->children = realloc(this->children, ++(this->size) * sizeof(context_tree));
	if (this->children == NULL) error(2, NULL);
	this->children[this->size - 1] = new;
}

void ctr_free_node(context_tree* this) {
	if (this->type == ct_string || this->type == ct_identifier) free(this->data);
	if (this->children != NULL) {
		for (int i = 0; i < this->size; i++) ctr_free_node(&(this->children[i]));
	}
	free(this->children);
}
