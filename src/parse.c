#include "parse.h"

int	is_higher_pres(int a, int b) {
	switch (a) {
		case tt_dot:
			return b >= tt_dot;
		case tt_let:
		case tt_val:
		case tt_fn:
			return b > tt_fn;
		case tt_ptr:
		case tt_addr:
		case tt_package:
		case tt_type:
			return b > tt_type;
		case tt_and:
			return b > tt_and;
		case tt_or:
			return b > tt_or;
		case tt_then:
		case tt_else:
		case tt_elif:
		case tt_if:
			return b > tt_elif;
		case tt_for:
		case tt_while:
		case tt_until:
			return b > tt_until;
		case tt_rarrow:
		case tt_larrow:
			return b > tt_rarrow;
		case tt_equ:
			return b > tt_equ;
		case tt_colon:
			return b > tt_colon;
		case tt_deq:
		case tt_leq:
		case tt_geq:
		case tt_greater:
		case tt_less:
			return b > tt_greater;
		case tt_minus:
		case tt_plus:
			return b > tt_minus;
		case tt_star:
		case tt_slash:
			return b > tt_star;
		default:
			return 0;
	}
}

char* name_token(int t) {
	switch (t) {
		case tt_backslash: return "\\";
		case tt_equ: return "=";
		case tt_dot: return ".";
		case tt_larrow: return "<-";
		case tt_rarrow: return "->";
		case tt_colon: return ":";
		case tt_deq: return "==";
		case tt_leq: return "<=";
		case tt_geq: return ">=";
		case tt_comma: return ",";
		case tt_greater: return ">";
		case tt_less: return "<";
		case tt_plus: return "+";
		case tt_minus: return "-";
		case tt_slash: return "/";
		case tt_star: return "*";
		case tt_lparen: return "(";
		case tt_rparen: return ")";
		case tt_lbracket: return "[";
		case tt_rbracket: return "]";
		case tt_sof: return "SOF";
		case tt_eof: return "EOF";
		case tt_ptr: return "ptr";
		case tt_addr: return "addr";
		case tt_type: return "type";
		case tt_package: return "package";
		case tt_then: return "then";
		case tt_if: return "if";
		case tt_elif: return "elif";
		case tt_else: return "else";
		case tt_val: return "val";
		case tt_for: return "for";
		case tt_while: return "while";
		case tt_until: return "until";
		case tt_and: return "and";
		case tt_or: return "or";
		case tt_lcurly: return "{";
		case tt_rcurly: return "}";
		case tt_fn: return "fn";
		case tt_let: return "let";
		default: return "[?]";
	}
}

void print_token(token t) {
	switch (t.type) {
	case tt_id:
		printf("id:%s", (char*)t.data);
		break;
	case tt_number:
		printf("num:%s", (char*)t.data);
		break;
	case tt_string:
		printf("str:%s", (char*)t.data);
		break;
	default: 
		printf("%s", name_token((int)t.type));
		break;
	}
	//printf(" [%i, %i]", t.line, t.column);

}
char* __print_ast(ast_node tree, char* prefix, char* child_prefix) {
	printf("\n");
	printf("%s", prefix);
	print_token(tree.node);
	strcpy(prefix, child_prefix);

	for (int i = 0; i < tree.child_count; i++) {
		char n_prefix[256];
		char n_child_prefix[256];

		strcpy(n_prefix, prefix);
		strcpy(n_child_prefix, child_prefix);

		if (i == tree.child_count - 1){
			strcat(n_prefix, "└──");
			strcat(n_child_prefix, "   ");
		} else {
			strcat(n_prefix, "├──");
			strcat(n_child_prefix, "│  ");
		}
		__print_ast(tree.children[i], n_prefix, n_child_prefix);
	}
}
void print_ast(ast_node tree) {
	char prefix[256] = {'\0'};
	char child_prefix[256] = {'\0'};
	__print_ast(tree, prefix, child_prefix);
	printf("\n");
}
char* __print_ctr(context_tree tree, char* prefix, char* child_prefix) {
	printf("\n");
	printf("%s", prefix);
	switch (tree.type) {
	case ct_identifier:
		printf("id:%s", (char*)tree.data);
		break;
	case ct_integer:
		printf("int:%i", (int)tree.data);
		break;
	case ct_string:
		printf("str:%s", (char*)tree.data);
		break;
	case ct_binary_op:
		printf("op:%s", name_token((int)tree.data));
		break;
	default: 
		printf("tok:%i", (int)tree.type);
		break;
	}
	printf(" @ [%i:%i]", tree.type, tree.line, tree.column);
	strcpy(prefix, child_prefix);

	for (int i = 0; i < tree.size; i++) {
		char n_prefix[256];
		char n_child_prefix[256];

		strcpy(n_prefix, prefix);
		strcpy(n_child_prefix, child_prefix);

		if (i == tree.size - 1){
			strcat(n_prefix, "└──");
			strcat(n_child_prefix, "   ");
		} else {
			strcat(n_prefix, "├──");
			strcat(n_child_prefix, "│  ");
		}
		__print_ctr(tree.children[i], n_prefix, n_child_prefix);
	}
}
void print_ctr(context_tree tree) {
	char prefix[256] = {'\0'};
	char child_prefix[256] = {'\0'};
	__print_ctr(tree, prefix, child_prefix);
	printf("\n");
}

ast_node parse_token(ast_node* AST, ast_node proto_AST, int* pos, ast_node** mark_addr) {
	ast_node* sub_mark_addr = NULL;
	if (*mark_addr != NULL) {
		parse_token(*mark_addr, proto_AST, pos, &sub_mark_addr);
		*mark_addr = NULL;
	} else if (IS_BIN_OP(proto_AST.children[*pos].node.type)) {
		if (AST->child_count > 0 && is_higher_pres(AST->children[AST->child_count - 1].node.type, proto_AST.children[*pos].node.type)) {
			ast_node* final_node_address = &(AST->children[AST->child_count - 1]);

			while(is_higher_pres(final_node_address->children[final_node_address->child_count - 1].node.type, proto_AST.children[*pos].node.type)) {
				final_node_address = &(final_node_address->children[final_node_address->child_count - 1]);
			}

			ast_append_node(final_node_address, proto_AST.children[*pos]);
			ast_append_node(&(final_node_address->children[final_node_address->child_count-1]), 
							final_node_address->children[final_node_address->child_count-2]);
			ast_remove_node(final_node_address, final_node_address->child_count-2);

			*mark_addr = &(final_node_address->children[final_node_address->child_count-1]);
		} else {
			ast_append_node(AST, proto_AST.children[*pos]);
			ast_append_node(&(AST->children[AST->child_count - 1]), AST->children[AST->child_count - 2]);
			(*pos)++;
			parse_token(&(AST->children[AST->child_count - 1]), proto_AST, pos, &sub_mark_addr);
			ast_remove_node(AST, AST->child_count - 2);
		}
	} else if (IS_FRONT_BIN_OP(proto_AST.children[*pos].node.type)) {
		if (AST->child_count > 0 && is_higher_pres(AST->children[AST->child_count - 1].node.type, proto_AST.children[*pos].node.type)) {
			ast_node* final_node_address = &(AST->children[AST->child_count - 1]);

			while(is_higher_pres(final_node_address->children[final_node_address->child_count - 1].node.type, proto_AST.children[*pos].node.type)) {
				final_node_address = &(final_node_address->children[final_node_address->child_count - 1]);
			}

			ast_append_node(final_node_address, proto_AST.children[*pos]);
			ast_append_node(&(final_node_address->children[final_node_address->child_count-1]), 
							proto_AST.children[++(*pos)]);
			ast_remove_node(final_node_address, final_node_address->child_count-1);

			*mark_addr = &(final_node_address->children[final_node_address->child_count-1]);
		} else {
			ast_append_node(AST, proto_AST.children[*pos]);
			(*pos)++;
			parse_token(&(AST->children[AST->child_count - 1]), proto_AST, pos, &sub_mark_addr);
			
		}
	} else if (IS_UNARY(proto_AST.children[*pos].node.type)) {
		if (AST->child_count > 0 && 
			AST->children[AST->child_count - 1].node.type == tt_if &&
			(proto_AST.children[*pos].node.type == tt_then ||  proto_AST.children[*pos].node.type == tt_else)
			) {
			
			
			(*pos)++;
			parse_token(&(AST->children[AST->child_count - 1]), proto_AST, pos, &sub_mark_addr);
		} else if (AST->child_count > 0 && (is_higher_pres(AST->children[AST->child_count - 1].node.type, proto_AST.children[*pos].node.type))) {
			ast_node* final_node_address = &(AST->children[AST->child_count - 1]);
			
			while(is_higher_pres(final_node_address->children[final_node_address->child_count - 1].node.type, proto_AST.children[*pos].node.type)) {
				final_node_address = &(final_node_address->children[final_node_address->child_count - 1]);
			}

			ast_append_node(final_node_address, proto_AST.children[*pos]);
			ast_remove_node(final_node_address, 1);

			*mark_addr = &(final_node_address);
		} else {
			ast_append_node(AST, proto_AST.children[*pos]);
			(*pos)++;
			parse_token(&(AST->children[AST->child_count - 1]), proto_AST, pos, &sub_mark_addr);
		}
	}  else if (proto_AST.children[*pos].node.type == tt_id) {
		ast_append_node(AST, proto_AST.children[*pos]);
		if (proto_AST.children[(*pos)+1].node.type == tt_lparen ||
			proto_AST.children[(*pos)+1].node.type == tt_lbracket) {
			ast_append_node(&(AST->children[AST->child_count - 1]), proto_AST.children[++(*pos)]);}
	} else 
		ast_append_node(AST, proto_AST.children[*pos]);
}

ast_node parse_expr(ast_node proto_AST) {
	ast_node AST = ast_generate_node(proto_AST.node);
	ast_node* mark_addr = NULL; 
	for (int i = 0; i < proto_AST.child_count; i++) {
		parse_token(&AST, proto_AST, &i, &mark_addr);
	}

	return AST;
}

ast_node parse(token_list tlist) {
	ast_node proto_AST = ast_generate_node(tlist.list[0]);
	ast_node AST;
	// Parse any parenthesised sections first
	for (int i = 1; i < tlist.real_len; i++) {
		if (IS_BRACKET(tlist.list[i].type)) {
			token_list sublist;
			int lparen_pos[] = {tlist.list[i].line, tlist.list[i].column};
			int paren_count = 1;
			const int original_type = tlist.list[i].type;

			generate_token_list(&sublist, tlist.list[i++]);
			while (paren_count > 0) {
				if (tlist.list[i].type == original_type + 1)
					paren_count--;
				else if (tlist.list[i].type == original_type)
					paren_count++;
				else if (tlist.list[i].type == tt_eof)
					error(5, lparen_pos);
				if (paren_count > 0) append_token_list(&sublist, tlist.list[i]);
				i++;
			}
			ast_node p_sublist = parse(sublist);
			ast_append_node(&proto_AST, p_sublist);

			i--;
			free_token_list(&sublist);
		} else {
			ast_append_node(&proto_AST, ast_generate_node(tlist.list[i]));
		}
	}

	AST = parse_expr(proto_AST);

	return AST;
}


context_tree contextualize(ast_node ast) {
	context_tree context;

	if (IS_BIN_OP(ast.node.type) && ast.node.type != tt_equ && ast.node.type != tt_colon) {
		context = ctr_new_node(ct_binary_op, (void*) ast.node.type, 
				ast.node.line, ast.node.column);
	} else {
		switch(ast.node.type) {
			case tt_sof:
				context = ctr_new_node(ct_head, (char*) NULL, 
						ast.node.line, ast.node.column);
				break;
			case tt_number:
				context = ctr_new_node(ct_integer, (int) atoi( (char*) ast.node.data ), 
						ast.node.line, ast.node.column);
				break;
			case tt_string:
				context = ctr_new_node(ct_string, (char*) ast.node.data , 
						ast.node.line, ast.node.column);
				break;
			case tt_id:
				context = ctr_new_node(ct_identifier, ast.node.data, 
						ast.node.line, ast.node.column);
				break;
			case tt_lbracket:
				context = ctr_new_node(ct_array, NULL, 
						ast.node.line, ast.node.column);
				break;
			case tt_lparen:
				context = ctr_new_node(ct_expr, NULL, 
						ast.node.line, ast.node.column);
				break;
			case tt_lcurly:
				context = ctr_new_node(ct_block, NULL, 
						ast.node.line, ast.node.column);
				break;
			case tt_let:
				context = ctr_new_node(ct_declaration, NULL, 
						ast.node.line, ast.node.column);
				break;
			case tt_val:
				context = ctr_new_node(ct_constant, NULL, 
						ast.node.line, ast.node.column);
				break;
			case tt_fn:
				context = ctr_new_node(ct_function, NULL, 
						ast.node.line, ast.node.column);
				break;
			case tt_equ:
				context = ctr_new_node(ct_assignment, NULL, 
						ast.node.line, ast.node.column);
				break;
			case tt_if:
				context = ctr_new_node(ct_if, NULL, 
						ast.node.line, ast.node.column);
				break;
			case tt_colon:
				context = contextualize(ast.children[0]);
				ctr_append_node(&context, 
					contextualize(ast.children[1]));
				return context;
				break;
			default:
				context = ctr_new_node(ct_error, NULL, 
						ast.node.line, ast.node.column);
			}
	}

	for (int i = 0; i < ast.child_count; i++) {
		switch (ast.children[i].node.type) {
			case tt_colon:
				ctr_append_node(&context, 
					contextualize(ast.children[i].children[0]));
				ctr_append_node(&context.children[context.size-1], 
					contextualize(ast.children[i].children[1]));
				break;
			case tt_equ:
				if (context.type == ct_declaration || context.type == ct_function || context.type == ct_constant) {
					ctr_append_node(&context, 
						contextualize(ast.children[i].children[0]));
					ctr_append_node(&context, 
						contextualize(ast.children[i].children[1]));
				} else {
					ctr_append_node(&context, 
						contextualize(ast.children[i]));
				}
				break;
			case tt_eof:
				break;
			default:
				ctr_append_node(&context, 
					contextualize(ast.children[i]));
				break;
		}
	}

	return context;
}