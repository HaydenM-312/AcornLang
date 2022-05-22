CC = gcc
SRC = src/parse.c src/error.c src/file.c src/lexer.c src/main.c src/astnode.c

all: src/main.c
	$(CC) $(SRC) -o Acorn -I include 