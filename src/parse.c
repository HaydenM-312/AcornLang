#include "parse.h"

int	is_higher_pres(int a, int b) {
	switch (a) {
		case TT_AND:
		case TT_OR:
			return b > TT_OR;
		case TT_IF:
		case TT_ELIF:
		case TT_ELSE:
			return b > TT_ELSE;
		case TT_FOR:
		case TT_WHILE:
		case TT_UNTIL:
			return b > TT_UNTIL;
		case TT_RARROW:
		case TT_LARROW:
			return b > TT_RARROW;
		case TT_EQU:
			return b > TT_EQU;
		case TT_COLON:
			return b > TT_COLON;
		case TT_DEQ:
		case TT_LEQ:
		case TT_GEQ:
		case TT_GREATER:
		case TT_LESS:
			return b > TT_GREATER;
		case TT_MINUS:
		case TT_PLUS:
			return b > TT_MINUS;
		case TT_STAR:
		case TT_SLASH:
			return b > TT_STAR;
		default:
			return 0;
	}
}

char* name_token(int t) {
	switch (t) {
		case TT_BACKSLASH: return "\\";
		case TT_EQU: return "=";
		case TT_LARROW: return "<-";
		case TT_RARROW: return "->";
		case TT_COLON: return ":";
		case TT_DEQ: return "==";
		case TT_LEQ: return "<=";
		case TT_GEQ: return ">=";
		case TT_GREATER: return ">";
		case TT_LESS: return "<";
		case TT_PLUS: return "+";
		case TT_MINUS: return "-";
		case TT_SLASH: return "/";
		case TT_STAR: return "*";
		case TT_LPAREN: return "(";
		case TT_RPAREN: return ")";
		case TT_LBRACKET: return "[";
		case TT_RBRACKET: return "]";
		case TT_SOF: return "SOF";
		case TT_EOF: return "EOF";
		case TT_NOT: return "not";
		case TT_TYPE: return "type";
		case TT_IF: return "if";
		case TT_ELIF: return "elif";
		case TT_ELSE: return "else";
		case TT_FOR: return "for";
		case TT_WHILE: return "while";
		case TT_UNTIL: return "until";
		case TT_AND: return "and";
		case TT_OR: return "or";
		default: return "[?]";
	}
}

void print_token(token t) {
	switch (t.type) {
	case TT_ID:
		printf("id:  %s", (char*)t.data);
		break;
	case TT_INTEGER:
		printf("int: %i", (int)t.data);
		break;
	case TT_STRING:
		printf("\"%s\"", (char*)t.data);
		break;
	default: 
		printf("%s", name_token((int)t.type));
		break;
	}
	//printf(" [%i, %i]", t.line, t.column);

}

void print_ast(ast_node tree, int depth) {
	for (int i = 0; i < depth; i++) {
		printf("|  ");
	}
	printf("|-- ");
	print_token(tree.node);
	printf("\n");

	for (int i = 0; i < tree.child_count; i++) {
		print_ast(tree.children[i], depth + 1);
	}
}

ast_node parse_expr(ast_node proto_AST) {
	ast_node AST = generate_node(proto_AST.node);

	for (int i = 0; i < proto_AST.child_count; i++) {
		if (IS_BIN_OP(proto_AST.children[i].node.type)) {
			if (is_higher_pres(AST.children[AST.child_count - 1].node.type, proto_AST.children[i].node.type)) {
				ast_node* final_node_address = &(AST.children[AST.child_count - 1]);
				
				while(is_higher_pres(final_node_address->children[final_node_address->child_count - 1].node.type, proto_AST.children[i].node.type)) {
					final_node_address = &(final_node_address->children[final_node_address->child_count - 1]);
				}

				append_node(final_node_address, proto_AST.children[i]);
				append_node(&(final_node_address->children[final_node_address->child_count-1]), 
								final_node_address->children[final_node_address->child_count-2]);
				append_node(&(final_node_address->children[final_node_address->child_count-1]), 
								proto_AST.children[++i]);
				remove_node(final_node_address, 1);

			} else {
				append_node(&AST, proto_AST.children[i]);
				append_node(&(AST.children[AST.child_count - 1]), AST.children[AST.child_count - 2]);
				append_node(&(AST.children[AST.child_count - 1]), proto_AST.children[++i]);
				remove_node(&AST, AST.child_count - 2);
			}
		} else if (IS_FRONT_BIN_OP(proto_AST.children[i].node.type)) {
				append_node(&AST, proto_AST.children[i]);
				append_node(&(AST.children[AST.child_count - 1]), proto_AST.children[++i]);
				append_node(&(AST.children[AST.child_count - 1]), proto_AST.children[++i]);
		} else {
			append_node(&AST, proto_AST.children[i]);
		}
	}

	return AST;
}

ast_node parse_funcs(ast_node proto_AST) {
	ast_node AST = generate_node(proto_AST.node);

	for (int i = 0; i < proto_AST.child_count; i++) {
		printf("%i\n", i);
		append_node(&AST, proto_AST.children[i]);
		if (proto_AST.children[i].node.type == TT_ID) {
			int start_line = proto_AST.children[i].node.line;
			i++;
			while (proto_AST.children[i].node.line == start_line && IS_VALUE(proto_AST.children[i].node.type)) {
				append_node(&(AST.children[AST.child_count - 1]), proto_AST.children[i++]);
			}
			i--;
		}
	}
	return AST;
}

ast_node parse(token_list tlist) {
	ast_node proto_AST = generate_node(tlist.list[0]);
	ast_node mid_AST;
	ast_node AST;

	// Parse any parenthesised sections first
	for (int i = 1; i < tlist.real_len; i++) {
		if (tlist.list[i].type == TT_LPAREN) {
			token_list sublist;
			int lparen_pos[] = {tlist.list[i].line, tlist.list[i].column};
			int paren_count = 1;

			generate_token_list(&sublist, tlist.list[i++]);
			while (paren_count > 0) {
				switch (tlist.list[i].type) {
					case TT_RPAREN:
						paren_count--;
						break;
					case TT_LPAREN:
						paren_count++;
						break;
					case TT_EOF:
						error(5, lparen_pos);
						break;
				}
				if (paren_count > 0) append_token_list(&sublist, tlist.list[i]);
				i++;
			}
			ast_node p_sublist = parse(sublist);
			append_node(&proto_AST, p_sublist);

			i--;
			free_token_list(&sublist);
		} else if (tlist.list[i].type == TT_LBRACKET) {
			token_list sublist;
			int lparen_pos[] = {tlist.list[i].line, tlist.list[i].column};
			int paren_count = 1;

			generate_token_list(&sublist, tlist.list[i++]);
			while (paren_count > 0) {
				switch (tlist.list[i].type) {
					case TT_RBRACKET:
						paren_count--;
						break;
					case TT_LBRACKET:
						paren_count++;
						break;
					case TT_EOF:
						error(5, lparen_pos);
						break;
				}
				if (paren_count > 0) append_token_list(&sublist, tlist.list[i]);
				i++;
			}
			ast_node p_sublist = parse(sublist);
			append_node(&proto_AST, p_sublist);

			i--;
			free_token_list(&sublist);
		} else {
			append_node(&proto_AST, generate_node(tlist.list[i]));
		}
	}

	mid_AST = parse_funcs(proto_AST);
	AST = parse_expr(mid_AST);

	return AST;
}
