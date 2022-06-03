#include "lexer.h"

static bool not_quote(char c) { return c != '"'; }
static bool is_digit(char c) { return isdigit(c) || c == '.';}

static char* read_on_condition(bool condition (char), char* text, size_t* start) {
	char* buffer = malloc(1);
	int j;

	for (j = 0; condition(text[*start + j]); j++) {
		buffer = realloc(buffer, j + 2);

		buffer[j] = text[(*start) + j];
		buffer[j + 1] = '\0';

		if (buffer == NULL) error(2, NULL);
		if (text[*start + j] == '\n') error(4, NULL);
	}

	*start += j - 1;
	return buffer;
}

static int parse_char(char c1, char c2) {
	switch (c1) {
		case '(': return tt_lparen;
		case ')': return tt_rparen;
		case '[': return tt_lbracket;
		case ']': return tt_rbracket;
		case '{': return tt_lcurly;
		case '}': return tt_rcurly;
		case ',': return tt_comma;
		case '.': return tt_dot;
		case '+': return tt_plus;
		case '-': {
			switch (c2) {
				case '>': return tt_rarrow;
				default: return tt_minus;
			}
		}
		case '/': return tt_slash;
		case '\\': return tt_backslash;
		case '*': return tt_star;
		case '=': {
			switch (c2) {
				case '=': return tt_deq;
				default: return tt_equ;
			}
		}
		case '>': {
			switch (c2) {
				case '=': return tt_geq;
				default: return tt_greater;
			}
		}
		case '<': {
			switch (c2) {
				case '=': return tt_leq;
				case '-': return tt_larrow;
				default: return tt_less;
			}
		}
		case ':': return tt_colon;
		default: return tt_eof;
	}
}

token generate_token(enum TOKEN_TYPE _type, void* _data, int _line, int _column) {
	token t;
	t.type = _type;
	t.data = _data;
	t.line = _line;
	t.column = _column;

	return t;
}

void generate_token_list(token_list* _tlist, token intial) {
	_tlist->list = malloc(sizeof(token));
	_tlist->real_len = 1;
	_tlist->target_len = 1;

	(_tlist->list)[0] = intial;
}

void append_token_list(token_list* _tlist, token _token) {
	if (_tlist->real_len + 1 >= _tlist->target_len) {
		_tlist->target_len *= 2;
		_tlist->list = realloc(_tlist->list, 
			_tlist->target_len * sizeof(token));

		if (_tlist->list == NULL) {
			error(2, NULL);
		}
	}
	(_tlist->list)[(_tlist->real_len)++] = _token;
}

token_list* parse_to_token(char* text) {
	token_list* tlist = malloc(sizeof(token_list));
	generate_token_list(tlist, generate_token(tt_sof, NULL, 0, 0));
	int line = 1;
	int column = 1;
	for (size_t i = 0; text[i] != 0; i++, column++) {
			if (strchr(" \t", text[i]) != NULL) {
			}
			else if (text[i] == '\n') {
				line++;
				column = 1;
			}
			else if (parse_char(text[i], text[i+1]) != tt_eof) {
				append_token_list(tlist, generate_token(parse_char(text[i], text[i+1]), NULL, line, column));
				if (IS_MULTI_CHAR(parse_char(text[i], text[i+1]))) i++;
			} else if (text[i] == '"') {
				i++;
				char* buffer = read_on_condition(*not_quote, text, &i);
				i++;
				
				append_token_list(tlist, generate_token(tt_string, _strdup(buffer), line, column));

				free(buffer);
			} 
			else if (isalpha(text[i])) {
				char* buffer = read_on_condition(*isalnum, text, &i);

				if (strcmp("let", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_let, NULL, line, column));
				} else if (strcmp("fn", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_fn, NULL, line, column));
				} else if (strcmp("else", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_else, NULL, line, column));
				} else if (strcmp("elif", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_elif, NULL, line, column));
				} else if (strcmp("if", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_if, NULL, line, column));
				} else if (strcmp("for", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_for, NULL, line, column));
				} else if (strcmp("while", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_while, NULL, line, column));
				} else if (strcmp("until", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_until, NULL, line, column));
				} else if (strcmp("or", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_or, NULL, line, column));
				} else if (strcmp("and", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_and, NULL, line, column));
				} else if (strcmp("ptr", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_ptr, NULL, line, column));
				} else if (strcmp("addr", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_addr, NULL, line, column));
				} else if (strcmp("type", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_type, NULL, line, column));
				} else if (strcmp("package", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_package, NULL, line, column));
				} else if (strcmp("then", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_then, NULL, line, column));
				} else if (strcmp("val", buffer) == 0) {
					append_token_list(tlist, 
						generate_token(tt_val, NULL, line, column));
				}
				else {
					append_token_list(tlist, 
						generate_token(tt_id, _strdup(buffer), line, column));
				}

				free(buffer);
			} else if (is_digit(text[i])) {
				char* buffer = read_on_condition(*is_digit, text, &i);
				
				append_token_list(tlist, generate_token(tt_number, 
					_strdup(buffer), line, column));
				free(buffer);
			}
	}

	append_token_list(tlist, generate_token(tt_eof, NULL, 0, 0));

	return tlist;
}

void free_token_list(token_list* _tlist) {
	free(_tlist->list);
}