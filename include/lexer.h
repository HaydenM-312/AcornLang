#pragma once

#include "error.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum TOKEN_TYPE {
	// Control types
		tt_eof,
		tt_sof,
		tt_eol,
		tt_comma,
	// Literals
		tt_number,
		tt_string,
		tt_boolean,
		tt_id,
	// Unary operators
		tt_let,
		tt_val,
		tt_fn,
		tt_else,
		tt_then,
	// Ternary Operators
		tt_if,
		tt_elif,
	// Binary Operators
		tt_for,
		tt_while,
		tt_until,
		tt_equ,
		tt_dot,
		tt_colon,
		tt_larrow,
		tt_rarrow,
		tt_or,
		tt_and,
		tt_deq,
		tt_leq,
		tt_geq,
		tt_greater,
		tt_less,
		tt_plus,
		tt_minus,
		tt_slash,
		tt_backslash,
		tt_star,
	// Unary operators
		tt_ptr,
		tt_addr,
		tt_type,
		tt_package,
	// Parentheses
		tt_lparen,
		tt_rparen,
		tt_lbracket,
		tt_rbracket,
		tt_lcurly,
		tt_rcurly
};

enum CONTEXT_TYPES {
	ct_error = -1,
	ct_head,
	ct_integer,
	ct_float,
	ct_character,
	ct_boolean,
	ct_string,
	ct_identifier,
	ct_type,
	ct_binary_op,
	ct_unary_op,
	ct_if,
	ct_function,
	ct_declaration,
	ct_assignment,
	ct_constant,
	ct_array,
	ct_expr,
	ct_block
};

#define IS_MULTI_CHAR(t) 	t == tt_larrow || t == tt_rarrow || (t >= tt_deq && t <= tt_geq)
#define IS_BIN_OP(t) 		t >= tt_equ && t <= tt_star
#define IS_FRONT_BIN_OP(t) 	t >= tt_if && t <= tt_until && t != tt_else
#define IS_UNARY(t) 		(t >= tt_ptr && t <= tt_package) || (t >= tt_let && t <= tt_then)
#define IS_BRACKET(t) 		t >= tt_lparen && t <= tt_rcurly
#define IS_VALUE(t) 		t >= tt_number && t <= tt_id || IS_BRACKET(t)

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

token		 generate_token		 (enum TOKEN_TYPE, void*, int, int);
void		 generate_token_list (token_list*, token);
void		 append_token_list	 (token_list*, token);
void		 free_token_list	 (token_list*);

token_list*	 parse_to_token		 (char*);

static bool  isquote			 (char c);
static char* read_on_condition	 (bool (char), char*, size_t*);
static int	 parse_char			 (char, char);
static int	 get_keyword		 (int);