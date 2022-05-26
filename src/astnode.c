#include "astnode.h"

ast_node generate_node(token t) {
	ast_node AST;
	AST.node = t;
	AST.children = NULL;
	AST.child_count = 0;

	return AST;
}

void append_node(ast_node* AST, ast_node node) {
	AST->children = realloc(AST->children, ++(AST->child_count) * sizeof(ast_node));
	if (AST->children == NULL) error(2, NULL);
	AST->children[AST->child_count - 1] = node;
}

void change_node(ast_node* AST, int position, ast_node new_node) {
	AST->children[position] = new_node;
}

void remove_node(ast_node* AST, int position) {
	for (int i = position + 1; i < AST->child_count; i++) AST->children[i-1] = AST->children[i];
	AST->children = realloc(AST->children, --(AST->child_count) * sizeof(ast_node));
}

void free_node(ast_node* AST) {
	if ((AST->node.type == TT_STRING || AST->node.type == TT_ID) && AST->node.data != NULL) free(AST->node.data);
	if (AST->children != NULL) free_node(AST->children);
}
