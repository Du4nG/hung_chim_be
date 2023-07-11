#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



typedef struct Lexer_t {
    char* source;
    char cur_char;
    int32_t cur_pos;
    int32_t size;
} Lexer;

typedef struct TextFile_t {
    char* Text;
    char* cur_char;
    int32_t cur_line;
} Text;

typedef enum TokenType_t {
    EOF_t = -1,
	NEWLINE = 0,
	NUMBER = 1,
	IDENT = 2,
	STRING = 3,
	/*Keywords*/
	LABEL = 101,
	GOTO = 102,
	PRINT = 103,
	INPUT = 104,
	LET = 105,
	IF = 106,
	THEN = 107,
	ENDIF = 108,
	WHILE = 109,
	REPEAT = 110,
	ENDWHILE = 111,
	/*Operators*/
	EQ = 201,
	PLUS = 202,
	MINUS = 203,
	ASTERISK = 204, /* '*' */
	SLASH = 205,    /* '/' */
	EQEQ = 206,		/*  == */
	NOTEQ = 207,
	LT = 208,
	LTEQ = 209,
	GT = 210,
	GTEQ = 211
} TokenType;

typedef struct Token_t {
    char* text;
    TokenType kind;
} Token;



void hello_lexer();
FILE* read_file(char* path);
// long get_file_size(char *filename);
int show_file(FILE* ptr_file);
int32_t calc_file_size (FILE* p_file);
// char* get_source(FILE *ptr_file);
Lexer* get_source(FILE *p_file);
void next_char(Lexer* code);
char peek(Lexer* code);
Token* get_token(Lexer* code);
uint8_t iskeyword (char* string);
void print_code(Lexer* code);
void print_char (char* p_text);
char* token2string (int16_t token);


#endif