#include "lexer.h"

static bool not_isquote(char c) { return c != '"'; }

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
		case '(': return TT_LPAREN;
		case ')': return TT_RPAREN;
		case '[': return TT_LBRACKET;
		case ']': return TT_RBRACKET;
		case '+': return TT_PLUS;
		case '-': {
			switch (c2) {
				case '>': return TT_RARROW;
				default: return TT_MINUS;
			}
		}
		case '/': return TT_SLASH;
		case '\\': return TT_BACKSLASH;
		case '*': return TT_STAR;
		case '=': {
			switch (c2) {
				case '=': return TT_DEQ;
				default: return TT_EQU;
			}
		}
		case '>': {
			switch (c2) {
				case '=': return TT_GEQ;
				default: return TT_GREATER;
			}
		}
		case '<': {
			switch (c2) {
				case '=': return TT_LEQ;
				case '-': return TT_LARROW;
				default: return TT_LESS;
			}
		}
		case ':': return TT_COLON;
		default: return TT_EOF;
	}
}

static int get_keyword(int k) {
	if (k <= 5) 	 return k + 7;
	else if (k <= 7) return k + 11;
	return k + 21;
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
	generate_token_list(tlist, generate_token(TT_SOF, NULL, 0, 0));
	int line = 1;
	int column = 1;
	for (size_t i = 0; text[i] != 0; i++, column++) {
			if (strchr(" \t", text[i]) != NULL) {
			}
			else if (text[i] == '\n') {
				line++;
				column = 1;
			}
			else if (parse_char(text[i], text[i+1]) != TT_EOF) {
				append_token_list(tlist, generate_token(parse_char(text[i], text[i+1]), NULL, line, column));
				if (IS_MULTI_CHAR(parse_char(text[i], text[i+1]))) i++;
			} else if (text[i] == '"') {
				i++;
				char* buffer = read_on_condition(*not_isquote, text, &i);
				i++;
				
				append_token_list(tlist, generate_token(TT_STRING, _strdup(buffer), line, column));

				free(buffer);
			} 
			else if (isalpha(text[i])) {
				char* buffer = read_on_condition(*isalnum, text, &i);
				int is_key_flag = 0;


				for (int k = 0; keyword_list[k] != '\0'; k++) {
					if (strcmp(keyword_list[k], buffer) == 0) {
						append_token_list(tlist, 
							generate_token(get_keyword(k), NULL, line, column));
						is_key_flag = 1;
					}
				}

				if (!is_key_flag) {
					append_token_list(tlist, 
						generate_token(TT_ID, _strdup(buffer), line, column));
				}

				free(buffer);
			} else if (isdigit(text[i])) {
				char* buffer = read_on_condition(*isdigit, text, &i);
				
				append_token_list(tlist, generate_token(TT_INTEGER, 
					strtol(_strdup(buffer), NULL, 10), line, column));
				free(buffer);
			}
	}

	append_token_list(tlist, generate_token(TT_EOF, NULL, 0, 0));

	return tlist;
}

void free_token_list(token_list* _tlist) {
	free(_tlist->list);
}