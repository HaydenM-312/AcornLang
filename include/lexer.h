#pragma once

#include "error.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum TOKEN_TYPE {
	// Control types
		TT_EOF,
		TT_SOF,
		TT_EOL,
	// Literals
		TT_INTEGER,
		TT_STRING,
		TT_BOOLEAN,
		TT_ID,
	// Ternary Operators
		TT_IF,
		TT_ELIF,
		TT_ELSE,
	// Binary Operators
		TT_FOR,
		TT_WHILE,
		TT_UNTIL,
		TT_EQU,
		TT_COLON,
		TT_LARROW,
		TT_RARROW,
		TT_OR,
		TT_AND,
		TT_DEQ,
		TT_LEQ,
		TT_GEQ,
		TT_GREATER,
		TT_LESS,
		TT_PLUS,
		TT_MINUS,
		TT_SLASH,
		TT_BACKSLASH,
		TT_STAR,
	// Unary operators
		TT_NOT,
		TT_TYPE,
	// Parentheses
		TT_LPAREN,
		TT_RPAREN,
		TT_LBRACKET,
		TT_RBRACKET,
		TT_LCURLY,
		TT_RCURLY
};

#define IS_MULTI_CHAR(t) t == TT_LARROW || t == TT_RARROW || (t >= TT_DEQ && t <= TT_GEQ)
#define IS_BIN_OP(t) t >= TT_EQU && t <= TT_STAR
#define IS_FRONT_BIN_OP(t) t >= TT_IF && t <= TT_UNTIL && t != TT_ELSE
#define IS_UNARY(t) (t >= TT_NOT && t <= TT_TYPE) || t == TT_ELSE
#define IS_BRACKET(t) t >= TT_LPAREN && t <= TT_RCURLY
#define IS_VALUE(t) t >= TT_INTEGER && t <= TT_ID ||IS_BRACKET(t)

typedef struct __token {
	enum TOKEN_TYPE type;
	void* data;
	int line;
	int column;
} token;

typedef struct __token_list {
	token* list;
	size_t real_len;
	size_t target_len;
} token_list;

static char* keyword_list[] = {
	"if",
	"elif",
	"else",
	"for",
	"while",
	"until",
	"or",
	"and",
	"not",
	"type",
	'\0'
};

token		 generate_token		 (enum TOKEN_TYPE, void*, int, int);
void		 generate_token_list (token_list*, token);
void		 append_token_list	 (token_list*, token);
void		 free_token_list	 (token_list*);

token_list*	 parse_to_token		 (char*);

static bool  isquote			 (char c);
static char* read_on_condition	 (bool (char), char*, size_t*);
static int	 parse_char			 (char, char);
static int	 get_keyword		 (int);